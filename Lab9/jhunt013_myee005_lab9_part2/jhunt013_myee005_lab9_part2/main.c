/*
 * jhunt013_myee005_lab9_part2.c
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
#define NoNote 0.00

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

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

enum States{init, wait, releaseB1, releaseB2, releaseB3, on} state;
double frequencies[] = {NoNote, NoteC4, NoteD4, NoteE4, NoteF4, NoteG4, NoteA4, NoteB4, NoteC5 };
unsigned char count = 0;

void TickSM(){
	switch(state){
		case init:
			state = wait;
			break;
		case wait:
			if (GetBit(~PINA, 0)) {
				PWM_on();
				state = releaseB1;
			}
			else {
				state = wait;
			}

			break;
		case releaseB1:
			if (GetBit(~PINA, 0)) {
				state = releaseB1;
			}
			else {
				state = on;
			}
			break;
		case on:
			if (GetBit(~PINA, 1)) {
				if (count <= 7 ) {
					++count;
					set_PWM(frequencies[count]);
				}
				else {
					set_PWM(frequencies[count]);
				}
				state = releaseB2;
			}
			else if (GetBit(~PINA, 2)) {
				if (count >= 2) {
					--count;
					set_PWM(frequencies[count]);
				}
				else {
					set_PWM(frequencies[count]);
				}
				state = releaseB3;
			}
			else if (GetBit(~PINA, 0)) {
				PWM_off();
				state = wait;
			}
			break;
		case releaseB2:
			if (GetBit(~PINA, 1)) {
				state = releaseB2;
			}
			else {
				state = on;
			}
			break;
		case releaseB3:
			if (GetBit(~PINA, 2)) {
				state = releaseB3;
			}
			else {
				state = on;
			}
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
