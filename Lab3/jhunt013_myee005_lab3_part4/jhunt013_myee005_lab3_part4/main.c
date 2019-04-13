/*
 * jhunt013_myee005_lab3_part4.c
 *
 * Created: 4/9/2019 2:38:15 PM
 *	Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 * Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB, 
 * likewise take the lower nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//setup for input
	DDRB = 0xFF; PORTB = 0x00;	//setup for output
	DDRC = 0xFF; PORTC = 0x00;	//setup for output
	
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char i;
	
    while (1) 
    {
    for(i = 0; i < 4; i++){
	    if(GetBit(PINA,i)){ //checking 3 2 1 0 positions
		    tmpC = SetBit(tmpC, i + 4, 1); //writing to 7 6 5 4
	    }
    }
    for(i = 4; i < 8; i++){
	    if(GetBit(PINA,i)){	//checking 7 6 5 4
		    tmpB = SetBit(tmpB, i - 4, 1);	//writing to 3 2 1 0
	    }
    }
    PORTB = tmpB;
    PORTC = tmpC;
    }
    
    return 0;
    }
