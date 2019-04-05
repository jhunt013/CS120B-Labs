/*
 * jhunt013_lab2_part1.c
 *
 * Created: 4/4/2019 4:34:51 PM
 * Author : jenny
 */ 

#include <avr/io.h>

int main(void)
{
	DDRA = 0x00;	//configure input
	PORTA = 0xFF;	// must initialize input pins to 1

	DDRB = 0x01;	//initialize output
	//bit set to 1 is being configured for output
	PORTB = 0x00;	//initialize

	unsigned char PA;
	unsigned char PB;

	while(1){
		PA = PINA & 0x03;
		if(PA == 0x01){	// if PA0 == 1 && PA1 == 0 set output to 1
			PB = 0x01;	//set LED to 1
		}
		else {
			PB = 0x00;	//set LED to 0
		}
		PORTB = PB;
	}
	return 0;
}

