/*
 * jhunt013_myee005_lab4_part3.c
 *
 * Created: 4/10/2019 9:26:56 PM
*	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #4  Exercise # 3
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>

enum States {init, wait, pressNum, releaseNum, pressY, releaseY, lock}state;

void LockSM(){

	switch(state) {	//transitions
		case init:
		state = wait;
		break;
		case wait:
		if (PINA == 0x04) {
			state = pressNum;
		}
		else {
			state = wait;
		}
		break;
		case pressNum:
		if(PINA == 0x04){
			state = pressNum;
		}
		else {
			state = releaseNum;
		}
		break;
		case releaseNum:
			if (PINA == 0x02) {
				state = pressY;
			}
			else if (PINA == 0x00) {
				state = releaseNum;
			}
			else {
				state = wait;
			}
		break;
		case pressY:
			if (PINA == 0x02) {
				state = pressY;
			}
			else {
				state = releaseY;
			}
			break;
		case releaseY:
			if (PINA == 0x80) {
				state = lock;
			}
			else {
				state = releaseY;
			}
			break;
		case lock:
			if (PINA == 0x80) {
				state = lock;
			}
			else {
				state = wait;
			}
			break;		
		
		default:
		state = init;
		break;
	}

	switch(state){	//actions
		case init:
		PORTB = 0x00;
		PORTC = 0x01;
		break;
		case wait:
		PORTB = 0x00;
		PORTC = 0x02;
		break;
		case pressNum:
		PORTB = 0x00;
		PORTC = 0x03;
		break;
		case releaseNum:
		PORTB = 0x00;
		PORTC = 0x04;
		break;
		case pressY:
		PORTB = 0x01;
		PORTC = 0x05;
		break;
		case releaseY:
		PORTB = 0x01;
		PORTC = 0x06;
		break;
		case lock:
		PORTB = 0x00;
		PORTC = 0x07;
		break;

		default:
		PORTB = 0x00;
		PORTC = 0x01;
		break;
		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	while (1)
	{
		LockSM();
	}
}

