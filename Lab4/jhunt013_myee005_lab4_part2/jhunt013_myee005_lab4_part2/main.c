/*
 * jhunt013_myee005_lab4_part2.c
 *
 * Created: 4/10/2019 8:28:54 PM
*	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #4  Exercise # 2
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>

enum States {start, init, wait, reset, increment, decrement}state;
int oldPina1=  0; 
int oldPina2 = 0;

void CntSM(){

	
/*	if ( ((oldPina1 == 0) && ((PINA & 0x01) == 0x01) ) ||
	     ((oldPina2 == 0) && ((PINA & 0x02) == 0x02) ) ||
	     (PINA == 0x03) ){			 */
	
			switch(state) {	//transitions
				case start:
					state = init;
					break;
				case init:
					state = wait;
					break;
				case wait:
					if((PINA == 0x01) && (PORTC < 0x09)){
						if ((oldPina1 == 0) && ((PINA & 0x01) == 0x01) ){
							state = increment;							
						}
					}
					else if ((PINA == 0x02) && (PORTC > 0x00)){
						if ((oldPina2 == 0) && ((PINA & 0x02) == 0x02) ){
							state = decrement;
						}
					}
					else if (PINA == 0x03){
						state = reset;
					}
					else {
						state = wait;
					}
					break;
				case reset:
					if ((PINA & 0x01) == 0x00){
						state = wait;
					}
					//state = wait;
					break;
				case increment:
					state = wait;
					break;
				case decrement:
					state = wait;
					break;
				default:
					state = start;
					break;
			}
		
			switch(state){	//actions
				case start:
					break;
				case init:
					PORTC = 0x07;
					break;
				case wait:
					break;
				case reset:
					PORTC = 0x00;
					break;
				case increment:
					PORTC = PORTC + 1;
					break;
				case decrement:
					PORTC = PORTC - 1;
					break;
				default:
					break;
				
			}
/*	}*/
	oldPina1 = PINA & 0x01;
	oldPina2 = PINA & 0x02;
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    while (1) 
    {
		CntSM();
    }
}

