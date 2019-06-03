/*
 * nokia.c
 *
 * Created: 5/28/2019 6:53:22 PM
 * Author : jenny
 */ 

#define F_CPU 8250000UL
#include <avr/io.h>
#include "nokia5110.h"
#include "nokia5110.c"
//#include "nokia5110_chars.h"
#define b1 (~PINA & 0x01) //0001
#define b2 (~PINA & 0x02) //0010
#define b3 (~PINA & 0x04) //0100
#define b4 (~PINA & 0x8) //1000

enum States{start, menu, option1, option2, option3}state;

void menu_tick(){
	switch(state){
		case start:
			state = menu;
			break;
		case menu:
			 if(b1){
				state = option1;
			}
			else if(b2){
				state = option2;
			}
			else if(b3){
				state = option3;
			}
			else{
				state = menu;
			}
			break;
		case option1:
			if(b4){
				state = menu;
			}
			else{
				state = option1;
			
			}
			break;
		case option2:
			if(b4){
				state = menu;
			}
			else{
				state = option2;
				
			}
			break;
		case option3:
			if(b4){
				state = menu;
			}
			else{
				state = option3;
				
			}
			break;
		default:
			state = start;
	}
	switch(state){
		case menu:
		PORTD = 0xFF;
			nokia_lcd_clear();
			nokia_lcd_set_cursor(3,3);
			nokia_lcd_write_string("1.SOS", 1);
			nokia_lcd_set_cursor(3,17);
			nokia_lcd_write_string("2.hi", 1);
			nokia_lcd_set_cursor(3,35);
			nokia_lcd_write_string("3.CUSTOM TEXT", 1);
			nokia_lcd_render();
			
			break;
		case option1:
		PORTD = 0x01;
			nokia_lcd_clear();
			nokia_lcd_set_cursor(3,3);
			nokia_lcd_write_string("S O S", 1);
			nokia_lcd_render();
			break;
		case option2:
		PORTD = 0x02;
			nokia_lcd_clear();
			nokia_lcd_set_cursor(3,3);
			nokia_lcd_write_string("h i", 1);
			nokia_lcd_render();
			break;
		case option3:
		PORTD = 0x04;
			nokia_lcd_clear();
			nokia_lcd_set_cursor(3,3);
			nokia_lcd_write_string("insert text", 1);
			nokia_lcd_render();
			break;
		default:
			break;
		
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
    /* Replace with your application code */
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
	state = start;
	
    while (1) 
    {
		menu_tick();
    }
}

