/*
* eeprom.c
*
* Created: 5/27/2019 8:23:31 PM
* Author : jenny
*/

#define F_CPU 8250000UL
#define ONE_SEC (8150)
#define ONE_M_SEC (8.15)

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void printStr(char *str, int maxOut)
{
	int idx=0;
	while ( (str[idx] != 0) && (idx < maxOut) )
	{
		USART_Transmit(str[idx]);
		idx++;
	}
}

int main(void)
{
	USART_Init(50);
	USART_Transmit('a');


	char array[128]= {0};
	
	uint8_t ByteOfData ;
	uint8_t value;
	value = eeprom_read_byte ((const uint8_t*)0);	/* Read value from 64 address */
	eeprom_read_block(array, ((const uint8_t*)64), sizeof(array));
	printStr("read", 4);
	printStr(array,sizeof(array));
	
	_delay_ms(1);
	if(value > 'z'){
		value = '!';
	}
	else if(value < '!'){
		value = '!';
	}

	ByteOfData = value + 1;
	eeprom_update_byte (( uint8_t *) 0, ByteOfData );
	
	memset(array, 0, sizeof(array));
	printStr("write", 5);
	sprintf(array,"something %2x:%c\r\n", (int)value, value);
	printStr(array,sizeof(array));
	eeprom_update_block(array, ((uint8_t*)64), sizeof(array));
		
	while(1)
	{
	}
}