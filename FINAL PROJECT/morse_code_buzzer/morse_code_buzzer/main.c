#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "nokia5110.h"
#include "nokia5110.c"

#define F_CPU 8250000UL
#define b1 (~PINA & 0x01) //0001
#define b2 (~PINA & 0x02) //0010
#define b3 (~PINA & 0x04) //0100
#define b4 (~PINA & 0x8) //1000
#define DOT  273, 0,
#define DASH 273, 273, 273, 0,
#define END   -1
#define PLAY_SIZE 128
short play[PLAY_SIZE];

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
char user_string[3] = {'e', 'e', 'b'};
unsigned char array_size = 0x00;

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

void encode(char val)
{
	//printf("Encode letter %c\n", val);
	switch(val)
	{
		case 'a':
		case 'A':
		{
			short tBuf[] = { DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 5;
			return;
		}
		case 'b':
		case 'B':
		{ 
			short tBuf[] = { DASH DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 7;
			return;
		}
		case 'c':
		case 'C':
		{
			short tBuf[] = { DASH DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 9;
			return;
		}
		case 'd':
		case 'D':
		{
			short tBuf[] = { DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 6;
			return;
		}
		case 'e':
		case 'E':
		{
			short tBuf[] = { DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 2;
			return;
		}
		case 'f':
		case 'F':
		{
			short tBuf[] = { DOT DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 7;
			return;
		}
		case 'g':
		case 'G':
		{
			short tBuf[] = { DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 8;
			return;
		}
		case 'h':
		case 'H':
		{
			short tBuf[] = { DOT DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 5;
			return;
		}
		case 'i':
		case 'I':
		{
			short tBuf[] = { DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 3;
			return;
		}
		case 'j':
		case 'J':
		{
			short tBuf[] = { DOT DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 11;
			return;
		}
		case 'k':
		case 'K':
		{
			short tBuf[] = { DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 8;
			return;
		}
		case 'l':
		case 'L':
		{
			short tBuf[] = { DOT DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 7;
			return;
		}
		case 'm':
		case 'M':
		{
			short tBuf[] = { DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 7;
			return;
		}
		case 'n':
		case 'N':
		{
			short tBuf[] = { DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 5;
			return;
		}
		case 'o':
		case 'O':
		{
			short tBuf[] = { DASH DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 10;
			return;
		}
		case 'p':
		case 'P':
		{
			short tBuf[] = { DOT DASH DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 9;
			return;
		}
		case 'q':
		case 'Q':
		{
			short tBuf[] = { DASH DASH DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 11;
			return;
		}
		case 'r':
		case 'R':
		{
			short tBuf[] = { DOT DASH DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 6;
			return;
		}
		case 's':
		case 'S':
		{
			short tBuf[] = { DOT DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 4;
			return;
		}
		case 't':
		case 'T':
		{
			short tBuf[] = { DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 2;
			return;
		}
		case 'u':
		case 'U':
		{
			short tBuf[] = { DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 6;
			return;
		}
		case 'v':
		case 'V':
		{
			short tBuf[] = { DOT DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 7;
			return;
		}
		case 'w':
		case 'W':
		{
			short tBuf[] = { DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 8;
			return;
		}
		case 'x':
		case 'X':
		{
			short tBuf[] = { DASH DOT DOT DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 9;
			return;
		}
		case 'y':
		case 'Y':
		{
			short tBuf[] = { DASH DOT DASH DASH END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 11;
			return;
		}
		case 'z':
		case 'Z':
		{
			short tBuf[] = { DASH DASH DOT DOT END };
			memcpy(play, tBuf, sizeof(tBuf));
			array_size = 9;
			return;
		}
		case ')':
			{
				short tBuf[] = { END };
				memcpy(play, tBuf, sizeof(tBuf));
				array_size = 1;
				return;
			}
		
	}
}

void playit()
{
	int index = 0;
	PWM_on();
	while ( (play[index] != END) && (index < PLAY_SIZE))
	{
		set_PWM(play[index]);
		index++;
	}
	
}

enum States{Start, Menu, Option1, Option2, Option3, Encode, Off, Sequence}state;
unsigned char button = 0x00;
unsigned char count = 0;
unsigned char i = 0;
unsigned char var = 0;

void tick(){
	switch(state){
		case Start:
			count = 0;
			i = 0;
			PWM_on();
			set_PWM(0);
			state = Menu;
			break;
		case Menu:
		PORTD = 0x07;
			if(b1){
				state = Option1;
			}
			else if(b2){
				state =  Option2;
			}
			else if(b3){
				state = Option3;
			}
			break;
		case Option1:
		PORTD = 0x01;
		var = 1;
			if(b4){
				state = Start;
			}
				user_string[0] = 's';
				user_string[1] ='o';
				user_string[2] = 's';
				state = Encode;
			break;
		case Option2:
		PORTD = 0x2;
		var = 2;
			if(b4){
				state = Start;
			}
				user_string[0] = 'h';
				user_string[1] = 'i';
				user_string[2] = ')';
				state = Encode;
			break;
		case Option3:
		PORTD = 0x4;
		var = 3;
		if(b4){
			state = Start;
		}
			user_string[0] = 'a';
			user_string[1] = 'b';
			user_string[2] = 'c';
			state = Encode;
		break;
		case Encode:
		PORTD = 0xAA;
		if(b4){
			i = 0;
			state = Start;
		}
		if(i < sizeof(user_string)){
			encode(user_string[i]);
			i++;
			state = Sequence;
		}
		else{
			//set_PWM(0);
			state = Start;
		}
			break;
		case Sequence:
		PORTD = 0xFF;
			set_PWM(play[count]);
			if((count < array_size) && (play[count] != END)){
				count ++;
				state = Sequence;
			}
			else {
				count = 0;
				state = Encode;
			}
			break;
		case Off:
			PORTD = 0xF0;
			set_PWM(0);
			if(b4){
				state = Menu;
			}
			break;
	}
		
	
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	TimerOn();
	TimerSet(200);
	state = Start;
	
	//encode('z');
	//playit();
	
	while (1)
	{
		
		tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
}