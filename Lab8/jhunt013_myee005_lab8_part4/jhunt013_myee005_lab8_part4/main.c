/*
 * jhunt013_myee005_lab8_part4.c
 *
 * Created: 4/29/2019 5:00:06 PM
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
		if (temp > 0 && temp <= 104) {
			PORTB = 0x01;
		}
		else if (temp > 104 && temp <= 208) {
			PORTB = 0x03;
		}
		else if (temp > 208 && temp <= 312) {
			PORTB = 0x07;
		}
		else if (temp > 312 && temp <= 416) {
			PORTB = 0x0F;
		}
		else if (temp > 416 && temp <= 520) {
			PORTB = 0x1F;
		}
		else if (temp > 520 && temp <= 624) {
			PORTB = 0x3F;
		}
		else if (temp > 624 && temp <= 728) {
			PORTB = 0x7F;
		}
		else if (temp > 728) {
			PORTB = 0xFF;
		}
		
	}
}

