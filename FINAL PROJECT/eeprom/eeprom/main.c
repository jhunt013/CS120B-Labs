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

int main(void)
{
	USART_Init(50);
	
	/*
	char R_array[15],W_array[15] = "EEPROM TEST";
	memset(R_array,0,15);
	eeprom_busy_wait();		// Initialize LCD
	eeprom_write_block(W_array,0,strlen(W_array));	// Write W_array from EEPROM address 0
	eeprom_read_block(R_array,0,strlen(W_array));	// Read EEPROM from address 0
	
	//LCD_String(R_array);		 //Print Read_array on LCD
	USART_Transmit(R_array);
	*/
	char array[128]= {0};
		int index = 0;
	
	uint8_t ByteOfData ;
	uint8_t value;
	value = eeprom_read_byte ((const uint8_t*)64);	/* Read value from
	64 address */
	
	sprintf(array,"something %2x:%c\r\n", (int)value, value);
	while(array[index] != 0){
		USART_Transmit(array[index]);
		index++;
	}
	
	_delay_ms(1);
	if(value > 'z'){
		value = '!';
	}
	ByteOfData = value + 1;
	eeprom_update_byte (( uint8_t *) 64, ByteOfData );
	
	while(1)
	{
	}
}