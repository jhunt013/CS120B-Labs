/*	Partner 1: Jennifer Hunter jhunt013@ucr.edu
*	Partner 2: Marcus Yee myee005@ucr.edu
*	Lab Section: 028
*	Assignment: Lab #9  Exercise #3
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


void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25
#define NoNote 0.00

double note[] = {C4, C4, C4, C4, C4, C4, 0, D4, D4, D4, D4, D4, D4, D4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, 0 , F4, F4, F4, F4, 0, F4, F4, 0, F4, F4, F4, 0, C4, C4, C4, C4, 0, D4, D4, D4, D4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, 0 , F4, F4, F4, F4, 0, F4, F4, 0, F4, F4, F4, 0, C4, C4, C4, C4, 0, D4, D4, D4, D4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, 0 , F4, F4, F4, F4, 0, F4, F4, 0, F4, F4, F4, 0, F4, F4, F4, 0, F4, F4, F4, 0, E4, E4, E4, E4, E4, E4};
enum States{Start, Off, Sequence, Inc}state;
unsigned char button = 0x00;
unsigned char count = 0;

void tick(){
	button = ~PINA & 0x01;
	switch(state){
		case Start:
		state = Off;
		break;
		case Off:
		if(button == 0x01){
			PWM_on();
			state = Sequence;
		}
		else{
			state = Off;
		}
		break;
		case Sequence:
		if(count< 200){
			state = Inc;
		}
		else if(count == 200){
			count = 0;
			state = Off;
		}
		break;
		case Inc:
		count++;
		state = Sequence;
		break;
		default:
		state = Start;
		break;
	}
	switch(state){
		case Start:
		break;
		
		case Off:
		PWM_off();
		break;
		
		case Sequence:
		set_PWM(note[count]);
		break;
		case Inc:
		break;
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerOn();
	TimerSet(30);
	state = Start;
	while (1)
	{
		tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}
