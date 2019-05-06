/*
 * jhunt013_myee005_lab9_part1.c
 *
 * Created: 5/1/2019 5:40:57 PM
 *	Partner 1: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/io.h>

#define NoteC4 261.63
#define NoteD4 293.66
#define NoteE4 329.63
#define NoteF4 349.23
#define NoteG4 392.00
#define NoteA4 440.00
#define NoteB4 493.88
#define NoteC5 523.25

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

enum States{init, wait} state;
double frequencies[] = {0, NoteC4, NoteD4, NoteE4, NoteF4, NoteG4, NoteA4, NoteB4, NoteC5 };
unsigned char count = 0;
unsigned char Bstate = 0;

void TickSM(){
	switch(state){
		case init:
			state = wait;
			break;
		case wait:
			if (GetBit(~PINA, 0) && Bstate == 0) {
				PWM_on();
				Bstate = 1;
				
			}
			else if (GetBit(~PINA, 0) && Bstate == 1) {
				PWM_off();
				Bstate = 0;
				
			}
			else if (GetBit(~PINA, 1) && Bstate == 1) {
				if (count <= 7 ) {
					++count;
					set_PWM(frequencies[count]);
				}
				else {
					set_PWM(frequencies[count]);
				}
			}
			else if (GetBit(~PINA, 2) && Bstate == 1) {
				if (count >= 1) {
					--count;
					set_PWM(frequencies[count]);
				}
				else {
					set_PWM(frequencies[count]);
				}
			}
			else {
				state = wait;
			}
			state = wait;
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
	
	
    while (1) {
		TickSM();
	}
}