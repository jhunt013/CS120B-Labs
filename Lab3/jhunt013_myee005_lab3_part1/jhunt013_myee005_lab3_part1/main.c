/*
 * jhunt013_myee005_lab3_part1.c
 *
 * Created: 4/8/2019 10:40:40 PM
 *	Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *	Count the number of 1s on ports A and B and output that number on port C. 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {	//get bit function
	return ( (x & (0x01 << k) ) != 0);
}


int main(void)
{
    //setup and initialize ports
    DDRA = 0x00;	PORTA = 0xFF;
    DDRB = 0x00;	PORTB = 0xFF;	//A and B setup for input
    DDRC = 0xFF;	PORTC = 0x00;	//C setup for output
    
    unsigned char i;		//need for the for loop
    unsigned char counter;
    while (1) 
    {
    counter = 0;
    for(i = 0; i < 8; i++){
	    if(GetBit(PINA,i)){	//count 1s on PORTA
		    counter++;
	    }
	    if(GetBit(PINB,i)){	//count 1s on PORTB
		    counter++;
	    }
    }
    PORTC = counter;		//write value of counter to PORTC
    }
    return 0;
    }

