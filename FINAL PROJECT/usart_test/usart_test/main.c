#define F_CPU 8250000UL
#define ONE_SEC (8150)
#define ONE_M_SEC (8.15)

#include <avr/io.h>
#include <util/delay.h>

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
	USART_Init(50); //FIXME baud rate

	/* Replace with your application code */
	while (1)
	{
		USART_Transmit(USART_Receive()-1);
		//USART_Transmit('Z');
		//_delay_ms(1000);
	} //connect ch1 scope to pin 15 atmega to measure width of bit ~= 1/10 millisecond (9600 baud)
	//confirm baud rate, then use tutorial to transmit what you receive to echo input to terminal
	
	// OR 1/ time = 1/width == baud rate 
}