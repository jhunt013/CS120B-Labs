/*
 * jhunt013_lab2_part3.c
 *
 * Created: 4/4/2019 7:10:12 PM
 * Author : jenny
 */ 

#include <avr/io.h>


int main(void){
	DDRA = 0x00;	//configure input
	PORTA = 0xFF;	//must initialize input pins to 1

	DDRC = 0x8F;	//configure output
	//need access to first three bits --> 4'b = 0100 and last bit --> 1000
	PORTC = 0x00;	//initialize to 0

	
	unsigned char PC;

	while(1){
	unsigned char cnt_used = 0x00;	//holds number of available spaces
    	//check each bit to count number of available spaces
    	if((PINA & 0x01) == 0x01){	//check first bit (index 0)
        	cnt_used += 1;
    	}
    	if((PINA & 0x02) == 0x02){	//checking second bit (index 1)
        	cnt_used +=1;
    	}
    	if ((PINA & 0x03) == 0x03){	//check third bit (index 2)
        	cnt_used += 1;
    	}
    	if ((PINA & 0x04) == 0x04){	//check fourth bit (index 3)
        	cnt_used += 1;
    	}

    	//output number of available spaces
    	
    	if (cnt_used == 4){
        	PC = 0x80;
    	}
    	else {
        	PC = (4 - cnt_used);
    	}
    	PORTC = PC;

	}
	return 0;
}

