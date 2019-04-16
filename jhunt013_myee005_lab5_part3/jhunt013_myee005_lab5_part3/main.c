/*
 * jhunt013_myee005_lab5_part3.c
 *
 * Created: 4/16/2019 3:18:53 PM
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
 
 enum States {init, wait, sequence}state;
 unsigned int count;
 void CntSM() {
	 switch(state) {
		case init:
			count = 0;
			state = wait;
			PORTB = 0x00;
			break;
		case wait:
			if (GetBit(~PINA, 0)) {
				if (count == 14) {
					count = 0;
				}
				else {
					count = count + 1;
				}
				state = sequence;
			}
			else {
				state = wait;
			}
			break;
		case sequence:
			if (GetBit(~PINA, 0)) {
				state = sequence;
				if (count <= 7){
					for (unsigned int i = 0; i < count; i++) {
						PORTB = SetBit(PORTB, i, 1);
					}
				}
				else if(count > 7 && count <= 14) {
					for (unsigned int j = 0; j <= count -7; j++) {
						PORTB = SetBit(PORTB, 7 - j, 0);
					}
				}
			}
			else {
				state = wait;
			}
			break;
		default:
			state = wait;
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

