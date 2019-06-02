
/*  
   LCD16x2 8 bit ATmega16 interface
   http://www.electronicwings.com
*/



#define F_CPU 8000000UL					/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>						/* Include AVR std. library file */
#include <util/delay.h>					/* Include inbuilt defined Delay header file */

#define LCD_Data_Dir DDRB				/* Define LCD data port direction */
#define LCD_Command_Dir DDRC			/* Define LCD command port direction register */
#define LCD_Data_Port PORTB				/* Define LCD data port */
#define LCD_Command_Port PORTC			/* Define LCD data port */
#define RS PC0							/* Define Register Select (data reg./command reg.) signal pin */
#define RW PC1							/* Define Read/Write signal pin */
#define EN PC2							/* Define Enable signal pin */
 

void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);		/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);		/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);		/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);
}

void LCD_Char (unsigned char char_data)		/* LCD data write function */
{
	LCD_Data_Port= char_data;
	LCD_Command_Port |= (1<<RS);			/*RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);			/*RW=0 write operation */
	LCD_Command_Port |= (1<<EN);			/*Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(2);							/* data write delay */
}

void LCD_Init (void)						/* LCD Initialize function */
{
	LCD_Command_Dir = 0xFF;					/* Make LCD command port direction as o/p */
	LCD_Data_Dir = 0xFF;					/* Make LCD data port direction as o/p */
	_delay_ms(20);							/* LCD Power ON delay always >15ms */
	
	LCD_Command (0x38);						/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0C);						/* Display ON Cursor OFF */
	LCD_Command (0x06);						/* Auto Increment cursor */
	LCD_Command (0x01);						/* clear display */
	_delay_ms(2);							/* clear display command delay> 1.63 ms */
	LCD_Command (0x80);						/* cursor at home position */
}


void LCD_String (char *str)							/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)							/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);					/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);					/* Command of first row and required position<16 */
	LCD_String(str);								/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);								/* clear display */
	LCD_Command (0x80);								/* cursor at home position */
}
 

void LCD_Custom_Char (unsigned char loc, unsigned char *msg)
{
	unsigned char i;
	if(loc<8)
	{
		LCD_Command (0x40 + (loc*8));  /* Command 0x40 and onwards forces the device to point CGRAM address */
		for(i=0;i<8;i++)             /* Write 8 byte for generation of 1 character */
		LCD_Char(msg[i]);
	}
}


int main()
{
	char i;
	
	unsigned char Character1[8] = { 0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00 };   //Custom char set for alphanumeric LCD Module
	unsigned char Character2[8] = { 0x04, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
	unsigned char Character3[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 };
	unsigned char Character4[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 };
	unsigned char Character5[8] = { 0x01, 0x03, 0x05, 0x09, 0x09, 0x0B, 0x1B, 0x18 };
	unsigned char Character6[8] = { 0x0A, 0x0A, 0x1F, 0x11, 0x11, 0x0E, 0x04, 0x04 };
	unsigned char Character7[8] = { 0x00, 0x00, 0x0A, 0x00, 0x04, 0x11, 0x0E, 0x00 };
	unsigned char Character8[8] = { 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00 };

	LCD_Init();
	
	
	LCD_Custom_Char(0, Character1);                    /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);                    /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character3);                    /* Build Character3 at position 2 */
	LCD_Custom_Char(3, Character4);                    /* Build Character4 at position 3 */
	LCD_Custom_Char(4, Character5);                    /* Build Character5 at position 4 */
	LCD_Custom_Char(5, Character6);                    /* Build Character6 at position 5 */
	LCD_Custom_Char(6, Character7);                    /* Build Character6 at position 6 */
	LCD_Custom_Char(7, Character8);                    /* Build Character6 at position 7 */

	LCD_Command(0x80);							//cursor at home position
	LCD_String("Custom char LCD");
	LCD_Command(0xc0);
	
	for(i=0;i<8;i++)																		//function will send data 1 to 8 to lcd
	{
		LCD_Char(i);																		//char at 'i'th position will display on lcd
		LCD_Char(' ');																		//space between each custom char.
	}
	while(1);
return 0;
}
