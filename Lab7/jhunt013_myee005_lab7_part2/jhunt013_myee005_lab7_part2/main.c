/*
* jhunt013_myee005_lab6_part2.c
*
* Created: 4/20/2019 11:58:37 AM
* Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
* Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
* Lab Section: 028
* Assignment: Lab #7  Exercise # 2
* I acknowledge all content contained herein, excluding template or example code, is my own original work.
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
volatile unsigned char TimerFlag = 0;	// TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;	// Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;	// Current internal count of 1ms ticks
unsigned char score = 5;

enum States{init, led_1, led_2, led_3, wait_1, button} state;

void TickSM(unsigned char timer,unsigned char press){
	switch(state){ //transitions
		case init:
		state = led_1;
		break;
		
		case led_1:
		if((press) == 1){
			score--;
			state = button;
		}
		else{
			state = led_2;
		}
		break;
		case led_2:
		if((press) == 1){
			score++;
			state = button;
		}
			else{
			state = led_3;
		}
		break;
		case led_3:
		if((press) == 1){
			score--;
			state = button;
		}
		else{
			state = init;
		}
		break;
		case button:
		if( (press) == 1){
			state = init;
		}
		else {
			state = button;
		}
		break;
	
		default:
		state = init;
		break;
	}


switch(state){ //actions
	case init:
	PORTC = 0x00;
	LCD_Cursor(1);
	LCD_WriteData(score + '0');

	break;
	case led_1:
	PORTC = 0x01;
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
	break;
	case led_2:
	PORTC = 0x02;
	if(score == 9){
		LCD_DisplayString(1, "WINNER");
		break;
	}
	else{
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
	}
		
	break;
	case led_3:
	PORTC = 0x04;
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
	break;
	case button:
	break;
	default:
	PORTC = 0x00;
		LCD_Cursor(1);
		LCD_WriteData(score + '0');
	break;
}



}
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.

	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
	
	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}
void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;	// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) {	// results in a more efficient compare
		TimerISR();	// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	LCD_init();
	TimerSet(75);
	TimerOn();
	
	unsigned char old_press = 0x00;
	unsigned char new_press = 0x00;
	
	while(1) {
		
		while (!TimerFlag){
			new_press = ~PINA & 0x01;
			
			if((old_press == 0x00) && (new_press == 0x01)){
				TickSM(0,1);
				
			}
			old_press = new_press;
		};	// Wait 1 sec
		TickSM(TimerFlag, ~PINA & 0x01);
		TimerFlag = 0;
		continue;
	}
	//return 0;
}