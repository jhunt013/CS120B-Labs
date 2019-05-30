#define F_CPU 8250000UL
#define ONE_SEC (8150)
#define ONE_M_SEC (8.15)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

char user_string[128] = {0};

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


void usart_get_string(){
	int index = 0;
	char tempBuf[128] = {0};
		
	while( (USART_Receive() != '\n') && (index < (sizeof(tempBuf) - 1)) ){
		tempBuf[index] = USART_Receive();
		index++;
	}
	memcpy(user_string, tempBuf, strlen(tempBuf));
}
int main(void)
{
	USART_Init(50); //FIXME baud rate
	printf("Encode letter %c\n");
	
	while (1)
	{
		//USART_Transmit(USART_Receive());


	} 
}