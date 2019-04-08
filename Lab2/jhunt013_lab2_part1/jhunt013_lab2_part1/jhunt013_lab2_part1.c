/*
 * jhunt013_lab2_part1.c
 *
 * Created: 4/4/2019 4:34:51 PM
 * Partner 1 Name & E-mail: Jennifer Hunter, jhunt013@ucr.edu
 * Lab Section: 028
 * Assignment: Lab #2  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 * Garage open at night-- A garage door sensor connects to PA0 (1 means door open), and a light sensor connects to PA1 (1 means light is sensed). 
 * Write a program that illuminates an LED connected to PB0 (1 means illuminate) if the garage door is open at night.

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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

