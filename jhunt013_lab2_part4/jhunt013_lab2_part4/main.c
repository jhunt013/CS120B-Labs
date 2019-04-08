/*
 * jhunt013_lab2_part4.c
 *
 * Created: 4/7/2019 7:26:25 PM
 * Author : jenny
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

