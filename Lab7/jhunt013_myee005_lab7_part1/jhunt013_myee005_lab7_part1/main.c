/*
 * jhunt013_myee005_lab7_part1.c
 *
 * Created: 4/24/2019 11:18:38 PM
 * Partner 1 Name & E-mail: Jennifer Hunter jhunt013@ucr.edu
 * Partner 2 Name & E-mail: Marcus Yee myee005@ucr.edu
 * Lab Section: 028
 * Assignment: Lab #7 Exercise # 1
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>
#include "io.c" // included from added directory

enum States {start, init, wait, reset, release_1, increment, release_2, decrement, release_3}state;
unsigned char count;

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

volatile unsigned char TimerFlag = 0;	// TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;	// Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;	// Current internal count of 1ms ticks

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

void CntSM(){

	switch(state) {	//transitions
		case start:
		state = init;
		break;
		case init:
		count = 0;
		state = wait;
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
		break;
		case wait:
		if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
			state = reset;
		}
		else if (GetBit(~PINA, 0)) {
			state = increment;
		}
		else if (GetBit(~PINA, 1)) {
			state = decrement;
		}
		else {
			state = wait;
		}
		break;
		case reset:
		if(GetBit(~PINA, 0) && GetBit(~PINA, 1)){
			state = reset;
			count = 0;
		}
		else {
			state = release_1;
		}
		break;
		case release_1:
		count = 0;
		state = wait;
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
		break;
		
		case increment:
		if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
			state = reset;
			count = 0;
		}
		else if(GetBit(~PINA, 0)){
			if (count >= 9) {
				count = 9;
			}
			else {
				++count;
			}
			LCD_Cursor(1);
			LCD_WriteData(count + '0');
			state = increment;
			while (!TimerFlag){};	// Wait 1 sec
			TimerFlag = 0;
		}
		else {
			state = release_2;
		}
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
		break;
		case release_2:
		state = wait;
		break;
		
		case decrement:
		if (GetBit(~PINA, 0) && GetBit(~PINA, 1)) {
			state = reset;
			count = 0;
		}
		else if(GetBit(~PINA, 1)){
			state = decrement;
		}
		else {
			state = release_3;
		}
		break;
		case release_3:
		if (count <= 0) {
			count = 0;
		}
		else {
			--count;
		}
		state = wait;
		LCD_Cursor(1);
		LCD_WriteData(count + '0');
		break;
		
		default:
		state = init;
		break;
	}

}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // buttons
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	// Initializes the LCD display
	LCD_init();
	
	TimerSet(250);
	TimerOn();
	
	while(1) {
		CntSM();

	}

}