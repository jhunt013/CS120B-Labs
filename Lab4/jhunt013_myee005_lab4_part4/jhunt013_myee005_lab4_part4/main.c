/*
 * jhunt013_myee005_lab4_part4.c
 *
 * Created: 4/11/2019 5:16:54 PM
*	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #4  Exercise # 4
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>

enum States {init, wait, pressNum, releaseNum, pressY, releaseY, lock, lockNum, releaseNum_2, lockY}state;

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
			else if (PINA == 0x04) {
				state = lockNum;
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
		case lockNum:
			if(PINA == 0x04){
				state = lockNum;
			}
			else {
				state = releaseNum_2;
			}
			break;
		case releaseNum_2:
			if (PINA == 0x02) {
				state = lockY;
			}
			else if (PINA == 0x00) {
				state = releaseNum_2;
			}
			else {
				state = releaseY;
			}
			break;
		case lockY:
			if (PINA == 0x02) {
				state = lockY;
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
		case lockNum:
			PORTB = 0x01;
			PORTC = 0x08;
			break;
		case releaseNum_2:
			PORTB = 0x01;
			PORTC = 0x09;
			break;
		case lockY:
			PORTB = 0x00;
			PORTC = 0x0A;
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

