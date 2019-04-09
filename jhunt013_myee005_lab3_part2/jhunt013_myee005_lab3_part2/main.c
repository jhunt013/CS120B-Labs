/*
 * jhunt013_myee005_lab3_part2.c
 *
 * Created: 4/8/2019 10:51:10 PM
 *	Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). 
 * A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. 
 * If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. 
 * Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0. 
 * Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less. 
 *	I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

/* fuel level: 	1-2	--> PC6, PC5				empty
		3-4	--> PC6, PC5, PC4

		5-6	--> PC5, PC4, PC3
		7-9 	--> PC5, PC4, PC3, PC2
		10-12	--> PC5, PC4, PC3, PC2, PC1
		13-15	--> PC5, PC4, PC3, PC2, PC1, PC0	full
	(if fuel level is <= 4 	--> PC6) 
	PC: 7 *6* 5 4    3 2 1 0 */

#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//setup PORTA for input
    DDRC = 0xFF; PORTC = 0x00;	//setup PORTB for output
    
    unsigned char tmpC = 0x00;
    unsigned char fuel_level;
    
    while (1) 
    {
    fuel_level = PINA & 0x0F;
    
    //check fuel level
    if((fuel_level == 0x01) || (fuel_level == 0x02)){	//1-2
	    tmpC = 0x60;	//0110 0000
    }
    else if ((fuel_level == 0x03) || (fuel_level == 0x04)){	//3-4
	    tmpC = 0x70;	//0111 0000
    }
    else if ((fuel_level == 0x05) || (fuel_level == 0x06)){	//5-6
	    tmpC = 0x38;	//0011 1000
    }
    else if ((fuel_level == 0x07) || (fuel_level == 0x08) || (fuel_level == 0x09)){	//7-9
	    tmpC = 0x3C;	//0011 1100
    }
    else if ((fuel_level == 0x0A) || (fuel_level == 0x0B) || (fuel_level == 0x0C)){	//10-12
	    tmpC = 0x3E;	//0011 1110
    }
    else if ((fuel_level == 0x0D) || (fuel_level == 0x0E) || (fuel_level== 0x0F)){	//13-15
	    tmpC = 0x3F;	//0011 1111
    }
    PORTC = tmpC;
    }
    return 0;
    }

