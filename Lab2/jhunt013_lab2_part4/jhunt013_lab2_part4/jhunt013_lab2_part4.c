/*
 * jhunt013_lab2_part4.c
 *
 * Created: 4/7/2019 7:26:25 PM
*	Partner 1 Name & E-mail: Jennifer Hunter, jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #2  Exercise #4 (challenge)
*	Exercise Description: [optional - include for your own benefit]
*	An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). 
*	Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. 
*	Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg. 
*	Can you also devise a way to inform the ride operator of the approximate weight using the remaining bits on D? 
*	(Hint: Use two intermediate variables to keep track of weight, one of the actual value and another being the shifted weight. 
*	Binary shift right by one is the same as dividing by two and binary shift left by one is the same as multiplying by two.)

*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>
#include <math.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0x00;
    DDRC = 0x00; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
	unsigned char actual_weight = 0x00;
	unsigned char shift_weight = 0x00;
	
	unsigned char temp_d = 0x00;
	
    
    
   
    while (1) 
    {actual_weight = PINA + PINB + PINC;
	    if (actual_weight > 140){
		    temp_d = (temp_d & 0xFE) | 0x01;
	    }
	    if ( abs(PINA - PINC) > 80){
		    temp_d = (temp_d & 0xFD) | 0x02;
	    }
	    
	    shift_weight = actual_weight * 4; //left shift twice
	    temp_d = temp_d | shift_weight;
	    PORTD = temp_d;
    }
	return 0;
}

