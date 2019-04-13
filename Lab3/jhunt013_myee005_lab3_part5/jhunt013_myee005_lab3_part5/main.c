/*
 * jhunt013_myee005_lab3_part5.c
 *
 * Created: 4/9/2019 2:47:06 PM
*	Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #3  Exercise # 5
*	Exercise Description: [optional - include for your own benefit]
* A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511) and connects to input PD7..PD0PB0 on the microcontroller. 
* If the weight is equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. If the weight is above 5 but below 70, 
* the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1. 
* (Neither B1 nor B2 should be set if the weight is 5 or less, as there is no passenger).

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
    DDRB = 0xFE; PORTB = 0x01;
    DDRD = 0x00; PORTD = 0xFF;
    
    unsigned char temp;
    unsigned char tmpB = 0x00;
    unsigned char tmpD = 0x00;
    unsigned char tot_weight = 0x00;
    while (1) 
    {
    temp = 0x00;
	tmpB = PINB;
    tmpD = PIND;
    
    tot_weight = (tmpD * 2); //| tmpB;
    tot_weight = SetBit(tot_weight,0,GetBit(tmpB,0));
	
    if(tot_weight >= 70){
	    temp = SetBit (temp, 1, 1);
    }
    else if( (tot_weight > 5) && (tot_weight < 70)){
	    temp = SetBit(temp, 2, 1);
    }
    else if(tot_weight <= 5){
	    temp = SetBit (temp, 2, 0);
	    temp = SetBit (temp, 1, 0);
    }
    PORTB = temp;
    
    }
    return 0;
    }
