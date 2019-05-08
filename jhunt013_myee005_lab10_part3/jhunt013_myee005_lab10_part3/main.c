/*
 * jhunt013_myee005_lab10_part3.c
 *
 * Created: 5/1/2019 5:40:57 PM
 *	Partner 1: Jennifer Hunter jhunt013@ucr.edu
 *	Partner 2: Marcus Yee myee005@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab #10  Exercise #3
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

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
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


enum BL_States{BL_start, BL_blink} BL_state;
enum TL_States{TL_start, TL_blink} TL_state;
enum Button_States{Button_start, Button_press}Button_state;
//enum Combine_States{Comb_start, Comb_do}Comb_state;

unsigned char threeLED = 0x04; // 0000 0100
unsigned char blinkingLED = 0x08; // 0000 1000
unsigned char stay = 0x00;
unsigned char b = 0x00;	//button

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

void Tick_Button(){
	b = ~PINA & 0x01; //0100
	
	switch(Button_state){
		case Button_start:
			if(b){ //checking
				Button_state = Button_press;
				break;
			}
			else{
				Button_state = Button_start;
				break;
			}
		case Button_press:
			if(b){ //checking
				Button_state = Button_press;
				break;
			}
			else{
				Button_state = Button_start;
				break;
			}
		default:
			Button_state = Button_start;
			break;
	}
	switch(Button_state){
		case Button_start:
			set_PWM(0);
			break;
		case Button_press:
			PWM_on();
			set_PWM(261.63);
			break;
		default:
			set_PWM(0);
			break;
	}
}
int main(void)
{
    DDRB = 0xFF; PORTB = 0x00; //outputs
    DDRA = 0x00; PORTA = 0xFF; //inputs
	
    unsigned long ThreeLEDS_elapsed_time = 0;
    unsigned long BlinkingLED_elapsed_time = 0;
	unsigned long Button_elapsed_time = 0;
    const unsigned long period = 100;
    
    TimerSet(period);
    TimerOn();
	PWM_on();
    
	TL_state = TL_start;
	BL_state = BL_start;
	Button_state = Button_start;
	
	PORTB = threeLED & blinkingLED;
    while (1) 
    {
		if(ThreeLEDS_elapsed_time >= 300){
			Tick_TL();
			ThreeLEDS_elapsed_time = 0;
		}
		if(BlinkingLED_elapsed_time >= 1000){
			Tick_BL();
			BlinkingLED_elapsed_time = 0;
		}
		if(Button_elapsed_time >= 2){
			Tick_Button();
			Button_elapsed_time = 0;
		}
		PORTB = blinkingLED | threeLED ; //blink - pb3    threeled - pb0-pb2
		
		while(!TimerFlag){}
			TimerFlag = 0;
			
		ThreeLEDS_elapsed_time += period;
		BlinkingLED_elapsed_time += period;
		Button_elapsed_time += 1;
	}
}

