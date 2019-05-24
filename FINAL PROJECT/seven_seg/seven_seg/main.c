/*
* seven_seg.c
*
* Created: 5/23/2019 4:15:06 PM
* Author : jenny
*/

#include <avr/io.h>
#define F_CPU	4000000UL
#include <util/delay.h>

int main(void)
{
	//Setup
	DDRC=0xFF;    //All output
	PORTC=0xFF;   //All segments off

	//dp g f e d c b a
	// a b c d e f g dp
	

	//PORTC = 0b00010001; //A
	//PORTC = 0b11000001; //b   
	//PORTC = 0b01100011; //c  
	//PORTC = 0b10000101; //d
	//PORTC = 0b01100001; //E
	//PORTC = 0b01110001; //F
	//PORTC = 0b00001001; //g
	//PORTC = 0b11010001; //h
	//PORTC = 0b10011111; //I
	//PORTC = 0b10000111; //J
	//PORTC = 0b11110001; //k
	//PORTC = 0b11100011; //L
	//PORTC = 0b01010101; //m
	//PORTC = 0b11010101; //n
	//PORTC = 0b00000011; //O
	//PORTC = 0b00110001; //P
	//PORTC = 0b00011001; //q
	//PORTC = 0b01110011; //r
	//PORTC = 0b01001001; //S
	//PORTC = 0b10011101; //t
	//PORTC = 0b10000011; //U
	//PORTC = 0b11000111; //v
	//PORTC = 0b10101011; //w
	//PORTC = 0b01101101; //X
	//PORTC = 0b10001001; //y
	//PORTC = 0b00100101; //Z

	while(1)
	{
	}
}