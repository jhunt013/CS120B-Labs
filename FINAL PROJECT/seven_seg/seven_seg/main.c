/*
* seven_seg.c
*
* Created: 5/23/2019 4:15:06 PM
* Author : jenny
*/

#include <avr/io.h>
#define F_CPU	4000000UL
#include <util/delay.h>
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

enum States{Start, Letter}state;
short tmp_letter = 0xFF;

void seven_Tick(){ //transitions
	switch(state){
		case Start:
			state = Letter;
			break;
		case Letter:
			break;
		default:
			state = Start;
	}
	switch(state){
		case Start:
			PORTC = 0xFF;
			break;
		case Letter:
			encode('b');
			PORTC = tmp_letter;
			break;
		default:
		break;
	}
} 
void encode(short val)
{
	switch(val)
	{
		case 'a':
		case 'A':
		{
			tmp_letter = 0b00010001;
			return;
		}
		case 'b':
		case 'B':
		{ 
			tmp_letter = 0b11000001;
			return;
		}
		case 'c':
		case 'C':
		{
			tmp_letter = 0b01100011;
			return;
		}
		case 'd':
		case 'D':
		{
			tmp_letter = 0b10000101;
			return;
		}
		case 'e':
		case 'E':
		{
			tmp_letter = 0b01100001;
			return;
		}
		case 'f':
		case 'F':
		{
			tmp_letter = 0b01110001;
			return;
		}
		case 'g':
		case 'G':
		{
			tmp_letter = 0b00001001;
			return;
		}
		case 'h':
		case 'H':
		{
			tmp_letter = 0b11010001;
			return;
		}
		case 'i':
		case 'I':
		{
			tmp_letter = 0b10011111;
			return;
		}
		case 'j':
		case 'J':
		{
			tmp_letter = 0b10000111;
			return;
		}
		case 'k':
		case 'K':
		{
			tmp_letter =  0b11110001;
			return;
		}
		case 'l':
		case 'L':
		{
			tmp_letter = 0b11100011;
			return;
		}
		case 'm':
		case 'M':
		{
			tmp_letter = 0b01010101;
			return;
		}
		case 'n':
		case 'N':
		{
			tmp_letter = 0b11010101;
			return;
		}
		case 'o':
		case 'O':
		{
			tmp_letter = 0b00000011;
			return;
		}
		case 'p':
		case 'P':
		{
			tmp_letter = 0b00110001;
			return;
		}
		case 'q':
		case 'Q':
		{
			tmp_letter = 0b00011001;
			return;
		}
		case 'r':
		case 'R':
		{
			tmp_letter = 0b01110011;
			return;
		}
		case 's':
		case 'S':
		{
			tmp_letter = 0b01001001;
			return;
		}
		case 't':
		case 'T':
		{
			tmp_letter = 0b10011101;
			return;
		}
		case 'u':
		case 'U':
		{
			tmp_letter = 0b10000011;
			return;
		}
		case 'v':
		case 'V':
		{
			tmp_letter = 0b11000111;
			return;
		}
		case 'w':
		case 'W':
		{
			tmp_letter = 0b10101011;
			return;
		}
		case 'x':
		case 'X':
		{
			tmp_letter = 0b01101101;
			return;
		}
		case 'y':
		case 'Y':
		{
			tmp_letter = 0b10001001;
			return;
		}
		case 'z':
		case 'Z':
		{
			tmp_letter = 0b00100101;
			return;
		}
	}
}

int main(void)
{
	//Setup
	DDRC=0xFF;    //All output 
	PORTC=0x00;   //All segments off

	//dp g f e d c b a
	// a b c d e f g dp
	TimerOn();
	TimerSet(100);
	state = Start;

	while(1)
	{
		seven_Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}