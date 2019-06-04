/*
* volume_disp.c
*
* Created: 6/1/2019 1:37:39 AM
* Author : jenny
*/

#define F_CPU 8000000UL		/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include inbuilt defined Delay header file */
#include <string.h>
#include <stdio.h>
#include "io.c"

#define LCD_Data_Dir DDRC	/* Define LCD data port direction */
#define LCD_Command_Dir DDRD	/* Define LCD command port direction register */
#define LCD_Data_Port PORTC	/* Define LCD data port */
#define LCD_Command_Port PORTD	/* Define LCD data port */
#define RS PD6			/* Define Register Select signal pin */
#define RW PC1			/* Define Read/Write signal pin */
#define EN PD7			/* Define Enable signal pin */


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char (unsigned char char_data)  /* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);			/* Data write delay */
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;		/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;		/* Make LCD data port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command (0x38);		/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);		/* Display ON Cursor OFF */
	LCD_Command (0x06);		/* Auto Increment cursor */
	LCD_Command (0x01);		/* clear display */
	_delay_ms(2);			/* Clear display command delay> 1.63 ms */
	LCD_Command (0x80);		/* Cursor at home position */
}

void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
}

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		LCD_Command (0x40 + (loc*8));	/* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)	/* Write 8 byte for generation of 1 character */
		LCD_Char(msg[i]);
	}
}

enum State{Start, Adjust}state;

void Tick_disp(){
	short range = 0x3FF;
	char array[128] = {0};
	char i;
	unsigned short temp = ADC;
	short val = (17 * temp) / range;
	unsigned char Character1[8] = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };	//open heart
	unsigned char Character2[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };	//phone
	//unsigned char Character3[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 };	//bell
	unsigned char Character3[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 };	//speaker
	unsigned char Character4[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	//speaker
	unsigned char Character5[8] = { 0x01, 0x03, 0x05, 0x09, 0x09, 0x0B, 0x1B, 0x18 };	//music note
	unsigned char Character6[8] = { 0x0A, 0x0A, 0x1F, 0x11, 0x11, 0x0E, 0x04, 0x04 };	//plug
	unsigned char Character7[8] = { 0x00, 0x00, 0x0A, 0x00, 0x04, 0x11, 0x0E, 0x00 };	//smile
	unsigned char Character8[8] = { 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00 };	//closed heart
	unsigned char Character9[8] = { 0xFF, 0xF0, 0x0F, 0x81, 0x18, 0xc3, 0x66, 0x24 };
	
	switch(state){
		case Start:
		
		state = Adjust;
		break;
		case Adjust:
		if(val > 0 && val <= 2){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character4);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character4);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character4);  /* Build Character6 at position 3 */
			LCD_Custom_Char(4, Character4);  /* Build Character6 at position 4 */
			LCD_Custom_Char(5, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<1;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 2 && val <=4){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character4);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character4);  /* Build Character6 at position 3 */
			LCD_Custom_Char(4, Character4);  /* Build Character6 at position 4 */
			LCD_Custom_Char(5, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<2;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 4 && val <= 6){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character4);  /* Build Character6 at position 3 */
			LCD_Custom_Char(4, Character4);  /* Build Character6 at position 4 */
			LCD_Custom_Char(5, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<3;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 6 && val <=8){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character5);  /* Build Character4 at position 3 */
			LCD_Custom_Char(4, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(5, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<4;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val >8 && val <= 10){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character5);  /* Build Character4 at position 3 */
			LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
			LCD_Custom_Char(5, Character4);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<5;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 10 && val < 12){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character5);  /* Build Character4 at position 3 */
			LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
			LCD_Custom_Char(5, Character5);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character4);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<6;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 12 && val <= 14){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character5);  /* Build Character4 at position 3 */
			LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
			LCD_Custom_Char(5, Character5);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character5);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character4);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<7;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		else if(val > 14 && val <= 16) {
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
			LCD_Custom_Char(1, Character5);  /* Build Character2 at position 1 */
			LCD_Custom_Char(2, Character5);  /* Build Character3 at position 2 */
			LCD_Custom_Char(3, Character5);  /* Build Character4 at position 3 */
			LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
			LCD_Custom_Char(5, Character5);  /* Build Character6 at position 5 */
			LCD_Custom_Char(6, Character5);  /* Build Character6 at position 6 */
			LCD_Custom_Char(7, Character5);  /* Build Character6 at position 7 */
			LCD_Command(0xc0);
			for(i=0;i<8;i++)		/* function will send data 1 to 8 to lcd */
			{
				LCD_Char(i);		/* char at 'i'th position will display on lcd */
				LCD_Char(' ');		/* space between each custom char. */
			}
		}
		break;
	}
}

int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	
	LCD_Init();
	ADC_init();
	LCD_Command(0x80);		/*cursor at home position */
	LCD_String("   volume ");
	
	TimerOn();
	TimerSet(25);
	state = Start;			
	
	while(1){
		Tick_disp();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}