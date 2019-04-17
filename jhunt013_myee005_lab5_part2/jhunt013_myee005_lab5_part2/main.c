/*
  *	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
  *	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
  *	Lab Section: 028
  *	Assignment: Lab #5  Exercise # 2
  *	I acknowledge all content contained herein, excluding template or example
  *	code, is my own original work.
 */ 


#include <avr/io.h>

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

enum States {start, init, wait, reset, release_1, increment, release_2, decrement, release_3}state;

void CntSM(){

	switch(state) {	//transitions
		case start:
			state = init;
			break;
		case init:
			state = wait;
			break;
		case wait:
			if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
				PORTB = 0x00;
				state = reset;
			}
			else if (GetBit(~PINA, 0)) {
				if (PORTB >= 0x09) {
					PORTB = 0x09;
				}
				else {
					PORTB = PORTB + 1;
				}
				state = increment;
			}
			else if (GetBit(~PINA, 1)) {
				if (PORTB <= 0x00) {
					PORTB = 0x00;
				}
				else {
					PORTB = PORTB - 1;
				}
				state = decrement;
			}
			else {
				state = wait;
			}
			break;
		case reset:
			if(GetBit(~PINA, 0) && GetBit(~PINA, 1)){
				state = reset;
			}
			else {
				state = release_1;
			}
			break;
		case release_1:
			state = wait;
		break;
		
		case increment:
		if(GetBit(~PINA, 0) && GetBit(~PINA, 1)){
			state = reset;
		}
		else if(GetBit(~PINA, 0)){
			state = increment;
		}
		else {
			state = release_2;
		}
		break;
		case release_2:
			state = wait;
		break;
		
		case decrement:
		if(GetBit(~PINA, 0) && GetBit(~PINA, 1)){
			state = reset;
		}
		else if(GetBit(~PINA, 1)){
			state = decrement;
		}
		else {
			state = release_3;
		}
		break;
		case release_3:
		state = wait;
		break;
		
		default:
			state = init;
			break;
	}

	switch(state){	//actions
		case start:
			PORTB = 0x00;
			break;
		case init:
			PORTB = 0x00;
			break;
		case wait:
//			if (PORTB >= 0x09) {
//				PORTB = 0x09;
//			}
//			else if (PORTB <= 0x00) {
//				PORTB = 0x00;
//			}
//			else {
//				PORTB = PORTB;
//			}
			break;
		case reset:
			PORTB = PORTB;
			break;
		case release_1:
			PORTB = 0x00;
			break;
		case increment:
			PORTB = PORTB;
			break;
		case release_2:
			//PORTB = PORTB + 1;
			break;
		case decrement:
			PORTB = PORTB;
			break;
		case release_3:
			//PORTB = PORTB - 1;
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
		CntSM();
    }
}