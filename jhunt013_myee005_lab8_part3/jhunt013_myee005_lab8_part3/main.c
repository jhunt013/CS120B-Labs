/*
 * jhunt013_myee005_lab8_part3.c
 *
 * Created: 4/29/2019 4:53:03 PM
 * Author : jenny
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	
	while (1) {
		unsigned short temp = ADC;
		if (temp >= 830 / 2) {
			PORTB = 0xFFFF;
			PORTD = 0xFFFF;
		}
		else {
			PORTB = 0x0000;
			PORTD = 0x0000;
		}
	}
}
