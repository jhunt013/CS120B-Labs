/*
 * jhunt013_myee005_lab4_part1.c
 *
 * Created: 4/10/2019 8:10:04 PM
*	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #4  Exercise # 1
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>

enum States {Start, init, PB0_ON, press_1, PB1_ON, press_2}state;

void TickSM(){

	switch(state) {	//transitions
		case Start:
			state = init;
			break;
		case init:
			state = PB0_ON;
			break;
		case PB0_ON:
		if((PINA & 0x01) == 0x01){
			state = press_1;
		}
		else {
			state = PB0_ON;
		}
		break;
		case press_1:
		if ((PINA & 0x01) == 0x01) {
			state = press_1;
		}
		else {
			state = PB1_ON;
		}
		break;
		case PB1_ON:
		if((PINA & 0x01) == 0x01){
			state = press_2;
		}
		else {
			state = PB1_ON;
		}
		break;
		case press_2:
		if ((PINA & 0x01) == 0x01) {
			state = press_2;
		}
		else {
			state = PB0_ON;
		}
		break;
		default:
		state = Start;
		break;
	}

	switch(state){	//actions
		case init:
		PORTB = 0x00;
		break;
		case PB0_ON:
		PORTB = 0x01;
		break;
		case press_1:
		PORTB = 0x02;
		break;
		case PB1_ON:
		PORTB = 0x02;
		break;
		case press_2:
		PORTB = 0x01;
		break;
		default:
		PORTB = 0x00;
		break;
		
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    while (1) 
    {
		TickSM();
    }
}

