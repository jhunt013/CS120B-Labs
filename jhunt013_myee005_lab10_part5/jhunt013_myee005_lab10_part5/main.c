/*
 * jhunt013_myee005_lab10_part5.c
 *
 * Created: 5/1/2019 5:40:57 PM
 *	Partner 1: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #10  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; 
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;	
	TIMSK1 = 0x02; 
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80; 
}

void TimerOff() {
	TCCR1B = 0x00; 
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

enum Cnt_States{Cnt_start, Cnt_init, Cnt_wait, Cnt_reset,Cnt_increment, Cnt_decrement} Cnt_state;
unsigned long new_period = 0x00; //for SM
//unsigned long const_timer = 0x00;
unsigned char count = 0x00;
unsigned long timer = 0x00; //overall timer

void Three_sec(){
	if(new_period == 1000 && timer >= 750){
		new_period = 400;
	}
	else if(new_period == 1 && timer >= 250){
		new_period = 1000;
	}
}

void CntSM() {
	switch(Cnt_state) {
		case Cnt_start:
			Cnt_state = Cnt_init;
			break;
		case Cnt_init:
			count = 0;
			Cnt_state = Cnt_wait;
			break;
		case Cnt_wait:
			if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
				Cnt_state = Cnt_reset;
			}
			else if (GetBit(~PINA, 0)) {
				Cnt_state = Cnt_increment;
			}
			else if (GetBit(~PINA, 1)) {
				Cnt_state = Cnt_decrement;
			}
			else {
				Cnt_state = Cnt_wait;
			}
			break;
		case Cnt_reset:
			if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
				Cnt_state = Cnt_reset;
				count = 0;
			}
			else {
				Cnt_state = Cnt_wait;
			}
			break;
		case Cnt_increment:
			if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
				Cnt_state = Cnt_reset;
				//count = 0;
			}
			else if (GetBit(~PINA, 0)) {
				Cnt_state = Cnt_increment;
				Three_sec();
			}
			else {
				Cnt_state = Cnt_wait;
			}
			break;
		case Cnt_decrement:
			if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
				Cnt_state = Cnt_reset;
				count = 0;
			}
			else if (GetBit(~PINA, 1)) {
				Cnt_state = Cnt_decrement;
				Three_sec();
			}
			else {
				Cnt_state = Cnt_wait;
			}
			break;
		default:
			Cnt_state = Cnt_init;
			break;
	}
	switch(Cnt_state) {
		case Cnt_start:
			break;
		case Cnt_init:
			//count = 0;
			break;
		case Cnt_wait:
			timer = 0;
			new_period = 1;
			break;
		case Cnt_reset:
			//count = 0;
			break;
		case Cnt_increment:
			if (count >= 9) {
				count = 9;
			}
			else {
				count++;
			}
			break;
		case Cnt_decrement:
			if (count <= 0) {
				count = 0;
			}
			else {
				count--;
			}
			break;
		default:
			break;
			
	}
}

int main(void)
{
    DDRB = 0xFF; PORTB = 0x00; //outputs
    DDRA = 0x00; PORTA = 0xFF; //inputs
	
    unsigned long cnt_elapsed_time = 1;
    const unsigned long period = 250;
    
    TimerSet(period);
    TimerOn();
	Cnt_state = Cnt_start;
	
	new_period = period;
    
    while (1) 
    {
		if (cnt_elapsed_time >= new_period) {
			CntSM();
			cnt_elapsed_time = 0;
		}
		PORTB = count;
		
		while(!TimerFlag){}
			TimerFlag = 0;
		
		cnt_elapsed_time += period;
		timer += period;
		
		
	}
}