#define F_CPU 8250000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "nokia5110.h"
#include "nokia5110.c"
#include "io.c"

#include <bit.h>
#include <timer.h>

#define b1 (~PINA & 0x08) //PA3 1000
#define b2 (~PINA & 0x04) //PA2 0100
#define b3 (~PINA & 0x02) //PA2 0010
#define b4 (~PINA & 0x8) //1000
double sound = 261.63;
//#define DOT  273, 0,
//#define DASH 273, 273, 273, 0,
#define DOT sound, 0,
#define DASH sound, sound, sound, 0,
#define END   -1
#define PLAY_SIZE 128
short play[PLAY_SIZE];

#define MAX_STRING (64)
char eeprom_string[MAX_STRING];

#define LCD_Data_Dir DDRC	/* Define LCD data port direction */
#define LCD_Command_Dir DDRD	/* Define LCD command port direction register */
#define LCD_Data_Port PORTC	/* Define LCD data port */
#define LCD_Command_Port PORTD	/* Define LCD data port */
#define RS PD6			/* Define Register Select signal pin */
#define RW PC1			/* Define Read/Write signal pin */
#define EN PD7			/* Define Enable signal pin */

#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25

//volatile unsigned char TimerFlag = 0;
//unsigned long _avr_timer_M = 1;
//unsigned long _avr_timer_cntcurr = 0;

char out_string[MAX_STRING];
unsigned char array_size = 0x00;

char user_string[MAX_STRING] = {0};

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

int usart_get_string(){
	int index = 0;
	char tmpBuf[128] = {0};
	
	tmpBuf[index] = USART_Receive();
	while (tmpBuf[index] != '\r' && (index < (sizeof(tmpBuf)-1) ) ) {
		USART_Transmit(tmpBuf[index]);
		index++;
		tmpBuf[index] = USART_Receive();
	}
	memcpy(user_string, tmpBuf, strlen(tmpBuf));
	return index;
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

void encode(char val)
{
	switch(val)
	{
		case 'a':
		case 'A':
		{
			short tBuf[] = { DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'b':
		case 'B':
		{
			short tBuf[] = { DASH DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'c':
		case 'C':
		{
			short tBuf[] = { DASH DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'd':
		case 'D':
		{
			short tBuf[] = { DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'e':
		case 'E':
		{
			short tBuf[] = { DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'f':
		case 'F':
		{
			short tBuf[] = { DOT DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'g':
		case 'G':
		{
			short tBuf[] = { DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'h':
		case 'H':
		{
			short tBuf[] = { DOT DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'i':
		case 'I':
		{
			short tBuf[] = { DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'j':
		case 'J':
		{
			short tBuf[] = { DOT DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'k':
		case 'K':
		{
			short tBuf[] = { DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'l':
		case 'L':
		{
			short tBuf[] = { DOT DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'm':
		case 'M':
		{
			short tBuf[] = { DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'n':
		case 'N':
		{
			short tBuf[] = { DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'o':
		case 'O':
		{
			short tBuf[] = { DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'p':
		case 'P':
		{
			short tBuf[] = { DOT DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'q':
		case 'Q':
		{
			short tBuf[] = { DASH DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'r':
		case 'R':
		{
			short tBuf[] = { DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 's':
		case 'S':
		{
			short tBuf[] = { DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 't':
		case 'T':
		{
			short tBuf[] = { DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'u':
		case 'U':
		{
			short tBuf[] = { DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'v':
		case 'V':
		{
			short tBuf[] = { DOT DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'w':
		case 'W':
		{
			short tBuf[] = { DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'x':
		case 'X':
		{
			short tBuf[] = { DASH DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'y':
		case 'Y':
		{
			short tBuf[] = { DASH DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case 'z':
		case 'Z':
		{
			short tBuf[] = { DASH DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		case ')':
		{
			short tBuf[] = { END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = sizeof(tBuf)/sizeof(tBuf[0]);
			return;
		}
		
	}
}

void playit()
{
	int index = 0;
	PWM_on();
	while ( (play[index] != END) && (index < PLAY_SIZE))
	{
		set_PWM(play[index]);
		index++;
	}
	
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


//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
	a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char count = 0;
unsigned char i = 0;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States {SM1_Start, Menu, Option1, Option2, Option3, Encode, Off, Sequence};

int SMTick1(int state){
	static int nb = 0;
	switch(state){
		case SM1_Start:
		count = 0;
		i = 0;
		PWM_on();
		set_PWM(0);
		state = Menu;
		break;
		case Menu:
		if(b1){
			state = Option1;
		}
		else if(b2){
			state =  Option2;
		}
		else if(b3){
			state = Option3;
		}
		break;
		case Option1:
		if(b4){
			state = SM1_Start;
		}
		out_string[0] = 's';
		out_string[1] ='o';
		out_string[2] = 's';
		nb = 3;
		state = Encode;
		break;
		case Option2:
		if(b4){
			state = SM1_Start;
		}
		out_string[0] = 'h';
		out_string[1] = 'i';
		nb = 2;
		
		state = Encode;
		break;
		case Option3:
		if(b4){
			state = SM1_Start;
		}
		nb = usart_get_string();
		memcpy(out_string, user_string, nb);
		state = Encode;
		break;
		case Encode:
		if(b4){
			i = 0;
			state = SM1_Start;
		}
		if(i < nb){
			encode(out_string[i]);
			i++;
			state = Sequence;
		}
		else{
			state = SM1_Start;
		}
		break;
		case Sequence:
		set_PWM(play[count]);
		if((count < array_size) && (play[count] != END)){
			count ++;
			state = Sequence;
		}
		else {
			count = 0;
			state = Encode;
		}
		break;
		case Off:
		set_PWM(0);
		if(b4){
			state = Menu;
		}
		break;
	}
	switch(state){
		case SM1_Start:
		break;
		case Encode:
		break;
		case Off:
		break;
		case Sequence:
		break;
		case Menu:
		nokia_lcd_clear();
		nokia_lcd_set_cursor(3,3);
		nokia_lcd_write_string("1.SOS", 1);
		nokia_lcd_set_cursor(3,17);
		nokia_lcd_write_string("2.hi", 1);
		nokia_lcd_set_cursor(3,35);
		nokia_lcd_write_string("3.CUSTOM TEXT", 1);
		nokia_lcd_render();
		break;
		case Option1:
		nokia_lcd_clear();
		nokia_lcd_set_cursor(3,3);
		nokia_lcd_write_string("S O S", 1);
		nokia_lcd_render();
		break;
		case Option2:
		nokia_lcd_clear();
		nokia_lcd_set_cursor(3,3);
		nokia_lcd_write_string("h i", 1);
		nokia_lcd_render();
		break;
		case Option3:
		{
			nokia_lcd_clear();
			nokia_lcd_set_cursor(3,3);
			nokia_lcd_write_string("insert text", 1);
			nokia_lcd_set_cursor(3,17);
			nokia_lcd_write_string("previous msg: ", 1);
			nokia_lcd_set_cursor(3,30);
			
			memset(eeprom_string,0, sizeof(eeprom_string));
			eeprom_read_block(eeprom_string, (const uint8_t *)64, MAX_STRING);
			int numBytes = strlen(eeprom_string);
			if ( numBytes >= MAX_STRING )
			{
				numBytes = MAX_STRING;
				eeprom_string[MAX_STRING-1] = 0; // Null terminate the truncated string
			}
			
			// Write the old string to nokia lcd
			nokia_lcd_write_string(eeprom_string, 1);
			nokia_lcd_render();
			
			//write current string to eeprom
			numBytes = strlen(user_string);
			if ( numBytes >= MAX_STRING )
			{
				numBytes = MAX_STRING;
			}
			user_string[numBytes - 1] = 0; // Null terminate the truncated string
			eeprom_update_block(user_string, (uint8_t *) 64, MAX_STRING);
			
			
			break;
		}
		
	}
	return state;
}
	
//Enumeration of states.
enum SM2_States {SM2_Start, Adjust};

int SMTick2(int state){
		short range = 0x3FF;
		char i;
		unsigned short temp = ADC;
		short val = (17 * temp) / range;
		
		switch(state){
			case SM2_Start:
			state = Adjust;
			break;
			case Adjust:
			if(val > 0 && val <= 2){
				sound = C4;
			}
			else if(val > 2 && val <=4){
				sound = D4;
			}
			else if(val > 4 && val <= 6){
				sound = E4;
			}
			else if(val > 6 && val <=8){
				sound = F4;
			}
			else if(val >8 && val <= 10){
				sound = G4;
			}
			else if(val > 10 && val < 12){
				sound = A4;
			}
			else if(val > 12 && val <= 14){
				sound = B4;
			}
			else if(val > 14 && val <= 16) {
				sound = C5;
			}
			LCD_Command(0xc0);
			 for ( i=0; i < 8; i++ )
			 {
				 if ( i <= val/2 )
				 {
					 LCD_Char(0);  // Which is really "Character 5"
					 LCD_Char(' ');  /* space between each custom char. */
				 }
				 else
				 {
					 LCD_Char(1);  // Which is really "Character 4"
					 LCD_Char(' ');  /* space between each custom char. */
				 }
			 }
			break;
		}
		return state;
	}

// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
	int main()
	{
		// Set Data Direction Registers
		// Buttons PORTA[0-7], set AVR PORTA to pull down logic
		DDRA = 0x00; PORTA = 0xFF;
		DDRB = 0xFF; PORTB = 0x00;
		
		USART_Init(50);
		ADC_init();
		LCD_Init();

		LCD_Command(0x80);		/*cursor at home position */
		LCD_String("    volume:  ");
		
		nokia_lcd_init();
		nokia_lcd_clear();
		nokia_lcd_power(1);
		
		unsigned char Character4[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	//speaker
		unsigned char Character5[8] = { 0x01, 0x03, 0x05, 0x09, 0x09, 0x0B, 0x1B, 0x18 };	//music note
		LCD_Custom_Char(0,Character5);     /* Build Character1 at position 0 */
        LCD_Custom_Char(1,Character4);     /* Build Character2 at position 1 */

		// . . . etc

		// Period for the tasks
		unsigned long int SMTick1_calc = 200;
		unsigned long int SMTick2_calc = 25;

		//Calculating GCD
		unsigned long int tmpGCD = 1;
		tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);

		//Greatest common divisor for all tasks or smallest time unit for tasks.
		unsigned long int GCD = tmpGCD;

		//Recalculate GCD periods for scheduler
		unsigned long int SMTick1_period = SMTick1_calc/GCD;
		unsigned long int SMTick2_period = SMTick2_calc/GCD;

		//Declare an array of tasks
		static task task1, task2;
		task *tasks[] = { &task1, &task2};
		const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

		// Task 1
		task1.state = SM1_Start;//Task initial state.
		task1.period = SMTick1_period;//Task Period.
		task1.elapsedTime = SMTick1_period;//Task current elapsed time.
		task1.TickFct = &SMTick1;//Function pointer for the tick.

		// Task 2
		task2.state = SM2_Start;//Task initial state.
		task2.period = SMTick2_period;//Task Period.
		task2.elapsedTime = SMTick2_period;//Task current elapsed time.
		task2.TickFct = &SMTick2;//Function pointer for the tick.

		// Set the timer and turn it on
		TimerSet(GCD);
		TimerOn();

		unsigned short i; // Scheduler for-loop iterator
		while(1) {
			// Scheduler code
			for ( i = 0; i < numTasks; i++ ) {
				// Task is ready to tick
				if ( tasks[i]->elapsedTime == tasks[i]->period ) {
					// Setting next state for task
					tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
					// Reset the elapsed time for next tick.
					tasks[i]->elapsedTime = 0;
				}
				tasks[i]->elapsedTime += 1;
			}
			
			while(!TimerFlag);
			TimerFlag = 0;
		}

		// Error: Program should not exit!
		return 0;
	}

