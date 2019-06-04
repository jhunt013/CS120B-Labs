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


int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	short range = 0x3FF;
	
	LCD_Init();
	ADC_init();
	char array[128] = {0};
	char i;
	
	unsigned char Character1[8] = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };	//open heart                                
	unsigned char Character2[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };	//phone
	unsigned char Character3[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 };	//bell
	unsigned char Character4[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 };	//speaker
	unsigned char Character5[8] = { 0x01, 0x03, 0x05, 0x09, 0x09, 0x0B, 0x1B, 0x18 };	//music note
	unsigned char Character6[8] = { 0x0A, 0x0A, 0x1F, 0x11, 0x11, 0x0E, 0x04, 0x04 };	//plug
	unsigned char Character7[8] = { 0x00, 0x00, 0x0A, 0x00, 0x04, 0x11, 0x0E, 0x00 };	//smile
	unsigned char Character8[8] = { 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00 };	//closed heart
	unsigned char Character9[8] = { 0xFF, 0xF0, 0x0F, 0x81, 0x18, 0xc3, 0x66, 0x24 };	
	

		
	while(1){
		unsigned short temp = ADC;
		short val = (17 * temp) / range;
		
		if(val > 0 && val <= 2){
			LCD_Custom_Char(0, Character5);  /* Build Character1 at position 0 */
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
	
	}
}