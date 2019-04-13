/*
 * jhunt0131_myee005_lab4_part5.c
 *
 * Created: 4/11/2019 5:36:49 PM
*	Partner 1 Name & E-mail: Marcus Yee myee005@ucr.edu
*	Partner 2 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #4  Exercise # 5
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
 */ 

#include <avr/io.h>
enum States{start, enter_num, lock_1, lock_2, wait, unlock}state;
char array[4] = {0,0,0,0};	
	
void Tick(){	//transitions
	switch(state){
		case start:
			state = wait;
			break;
			
		case wait:
			if((PINA & 0x04) == 0x04){
				array[0] = 0x04;				
				state = enter_num;
			}
			else{
				state = wait;
			}
			break;
			
		case enter_num:
			if(((PINA & 0x04) == 0x04) && (array[0] == 0x04)){		//must enter x to start	
				state = enter_num;
			}
			else if(((PINA & 0x01) == 0x01) && (array[0] == 0x04) && (array[2] == 0)){
				array[1] = 0x01;				
				state = enter_num;
			}
			else if(((PINA & 0x02) == 0x02) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x00)){
				array[2] = 0x02;				
				state = enter_num;

			}
			else if(((PINA & 0x02) == 0x02) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x02)){  //loops state	
				state = enter_num;
			}			
			else if(((PINA & 0x01) == 0x01) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x02) && (array[3] == 0x00)){
				array[3] = 0x01;				
				state = enter_num;
			}
			else{	//reset array
				array[0] = 0x00;
				array[1] = 0x00;
				array[2] = 0x00;
				array[3] = 0x00;
				state = wait;
			}
			break;
			
		case unlock:
			array[0] = 0x00;
			array[1] = 0x00;
			array[2] = 0x00;
			array[3] = 0x00;			
			state = lock_1;
			break;
			
		case lock_1:
			if((PINA & 0x80) == 0x80){				
				state = wait;
			}
			else if((PINA & 0x04) == 0x04){
				array[0] = 0x04;					
				state = lock_2;
			}
			else{
				state = lock_1;
			}
			break;
			
		case lock_2:
			if(((PINA & 0x04) == 0x04) && (array[0] == 0x04)){		//loop state waiting for the first x	
				state = lock_2;
			}
			else if(((PINA & 0x01) == 0x01) && (array[0] == 0x04) && (array[2] == 0)){
				array[1] = 0x01;				
				state = lock_2;
			}
			else if(((PINA & 0x02) == 0x02) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x00)){
				array[2] = 0x02;				
				state = lock_2;

			}
			else if(((PINA & 0x02) == 0x02) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x02)){  //loops state	
				state = lock_2;
			}			
			else if(((PINA & 0x01) == 0x01) && (array[0] == 0x04) && (array[1] == 0x01) && (array[2] == 0x02) && (array[3] == 0x00)){
				array[3] = 0x01;				
				state = wait;
			}
			else{
				array[0] = 0x00;
				array[1] = 0x00;
				array[2] = 0x00;
				array[3] = 0x00;
				state = lock_1;
			}
			break;
			
		default:
			state = start;
			break;
	}
	
	switch(state){	//actions
		case start:
			PORTC = 0x00;
			break;
			
		case wait:
			PORTB = 0x00;
			PORTC = 0x01;
			break;
			
		case enter_num:
			PORTC = 0x02;
			break;
			
		case unlock:
			PORTC = 0x03;
			PORTB = 0x01;
			break;
			
		case lock_1:
			PORTC = 0x04;
			break;
			
		case lock_2:
			PORTC = 0x05;
			break;
			
		default:
			break;
	}
}

int main(void)
{
	//setup
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    while (1) 
    {
		Tick();
    }
	return 0;
}
