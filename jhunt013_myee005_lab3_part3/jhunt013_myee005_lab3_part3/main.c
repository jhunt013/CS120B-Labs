/*
 * jhunt013_myee005_lab3_part3.c
 *
 *  Created: 4/8/2019 11:15:32 PM
 *  Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #3  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
*   In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, and PA6 is 1 if the driver's seatbelt is fastened. 
*   PC7 should light a "Fasten seatbelt" icon if a key is in the ignition, the driver is seated, but the belt is not fastened.
 *	I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

//fasten seatbelt: if (PA4 && PA5 && !PA6) --> PC7

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
    else if ((fuel_level == 0x0D) || (fuel_level == 0x0E) || (fuel_level == 0x0F)){	//13-15
	    tmpC = 0x3F;	//0011 1111
    }

    tmpC = tmpC & 0x7F;
    
    //fasten seatbelt sign
    if (GetBit(PINA,4) && GetBit(PINA,5) && !GetBit(PINA,6)){
	    tmpC = SetBit(tmpC,7,1);
    }
    PORTC = tmpC;
    }
    return 0;
    }

