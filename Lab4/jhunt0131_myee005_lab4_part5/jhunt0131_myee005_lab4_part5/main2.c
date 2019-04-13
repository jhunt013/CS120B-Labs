/*
 * jhunt0131_myee005_lab4_part5.c
 *
 * Created: 4/11/2019 5:36:49 PM
 * Author : jenny
 */ 

#include <avr/io.h>
enum States {init, wait, press, release, unlock, imlock}state;

void LockSM(){
	unsigned int count = 0;
	switch(state) {	//transitions
		case init:
		state = wait;
		break;
		case wait:
		if (count == 0 && PINA == 0x04) {
			state = press;
			++count;
		}
		else {
			state = wait;
		}
		break;
		case press:
		if (count == 0 && PINA == 0x04) {
			state = press;
		}
		else if (count == 1 && PINA == 0x01) {
			state = press;
		}
		else if (count == 2 && PINA == 0x02) {
			state = press;
		}
		else if (count == 3 && PINA == 0x01) {
			state = press;
		}
		else if (count == 5 && PINA == 0x04) {
			state = press;
		}
		else if (count == 6 && PINA == 0x01) {
			state = press;
		}
		else if (count == 7 && PINA == 0x02) {
			state = press;
		}
		else if (count == 8 && PINA == 0x01) {
			state = press;
		}
		else if (PINA == 0x00) {
			state = release;
		}
		else if (count >= 0 && count <= 4) {
			count = 0;
			state = wait;
		}
		else if (count >= 5 && count <=8) {
			count = 4;
			state = unlock;
		}
		
		break;
		case release:
		if (PINA == 0x00) {
			state = release;
		}
		else if (count == 1 && PINA == 0x01) {
			state = press;
			++count;
		}
		else if (count == 2 && PINA == 0x02) {
			state = press;
			++count;
		}
		else if (count == 3 && PINA == 0x01) {
			state = press;
			++count;
		}
		else if (count == 4) {
			state = unlock;
		}
		else if (count == 5 && PINA == 0x01) {
			state = press;
			++count;
		}
		else if (count == 6 && PINA == 0x02) {
			state = press;
			++count;
		}
		else if (count == 7 && PINA == 0x01) {
			state = press;
			++count;
		}
		else if (count == 8) {
			state = wait;
		}
		break;
		
		case unlock:
		if (count == 4 && PINA == 0x80) {
			state = imlock;
		}
		else if (count == 4 && PINA == 0x04) {
			state = press;
			++count;
		}
		else if (count == 4) {
			state = unlock;
		}
		else {
			state = unlock;
		}
		break;
		
		case imlock:
			if (count == 4 && PINA == 0x80) {
				state = imlock;
			}
			else if (PINA == 0x00) {
				state = wait;
			}
			else {
				state = wait;
			}
		
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
		case press:
			PORTB = PORTB;
			PORTC = 0x03;
			break;
		case release:
			PORTB = PORTB;
			PORTC = 0x04;
			break;
		case unlock:
			PORTB = 0x01;
			PORTC = 0x05;
			break;
		case imlock:
			PORTB = 0x00;
			PORTC = 0x06;
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