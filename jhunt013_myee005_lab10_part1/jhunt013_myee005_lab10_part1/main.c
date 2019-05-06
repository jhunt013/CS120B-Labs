/*
 * jhunt013_myee005_lab10_part1.c
 *
 * Created: 5/1/2019 5:40:57 PM
 *	Partner 1: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
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
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum BL_States{BL_start, BL_blink} BL_state;
enum TL_States{TL_start, TL_blink} TL_state;
//enum CombineLEDS_State{}CombineLEDS_state;

unsigned char threeLED = 0x04; // 0000 0100
unsigned char blinkingLED = 0x08; // 0000 1000
unsigned char stay = 0x00;

void Tick_TL(){
	switch(TL_state){
		case TL_start:
		TL_state = TL_blink;
		break;
		case TL_blink:
		TL_state = TL_blink;
		break;
		default:
		TL_state = TL_start;
		break;
	}
	switch(TL_state){
		case TL_start:
		threeLED = 0x04;
		break;
		case TL_blink:
		if(threeLED == 0x04){
			threeLED = 0x01;
			break;
		}
		else if(threeLED == 0x01){
			threeLED = 0x02;
			break;
		}
		else if(threeLED == 0x02){
			threeLED = 0x04;
			break;
		}
		else{
			break;
		}
		default:
		break;
	}
}
void Tick_BL(){
	switch(BL_state){
		case BL_start:
		BL_state = BL_blink;
		break;
		case BL_blink:
		BL_state = BL_blink;
		break;
		default:
		BL_state = BL_start;
		break;
	}
	switch(BL_state){
		case BL_start:
		blinkingLED = 0x08;
		break;
		case BL_blink:
		if(blinkingLED == 0x08){
			blinkingLED = 0x00;
			break;
		}
		else if(blinkingLED == 0x00){
			blinkingLED = 0x08;
			break;
		}
		else{
			break;
		}
		default:
		break;
	}
}
int main(void)
{
    DDRB = 0xFF; PORTB = 0x00;
    
    unsigned long ThreeLEDS_elapsed_time = 0;
    unsigned long BlinkingLED_elapsed_time = 0;
    const unsigned long period = 100;
    
    TimerSet(period);
    TimerOn();
    
	ThreeLEDS_state = ThreeLEDS_start;
	BlinkingLED_state = BlinkingLED_start;
	
	PORTB = threeLEDS & blinkingLED;
    while (1) 
    {
		if(ThreeLEDS_elapsed_time >= 1000){
			Tick_TL();
			ThreeLEDS_elapsed_time = 0;
		}
		if(BlinkingLED_elapsed_time >= 1000){
			Tick_BL());
			BlinkingLED_elapsed_time = 0;
		}
		PORTB = threeLEDS | blinkingLED;
		
		while(!TimerFlag){}
			TimerFlag = 0;
		ThreeLEDS_elapsed_time += period;
		BlinkingLED_elapsed_time += period;
	}
}

