/*
 * jhunt013_lab2_part2.c
 * Partner 1 Name & E-mail: Jennifer Hunter, jhunt013@ucr.edu
 * Lab Section: 028
 * Assignment: Lab #2  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 * Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space, of a four-space parking lot. 
 * Write a program that outputs in binary on port C the number of available spaces (Hint: declare a variable "unsigned char cntavail"
 * you can assign a number to a port as follows: PORTC = cntavail;).
 * Created: 4/4/2019 6:56:04 PM
 */ 

#include <avr/io.h>


int main(void)
{
   DDRA = 0x00;	//configure input
   PORTA = 0xFF;	//must initialize input pins to 1

   DDRC = 0x07;	//configure output
   //need access to first three bits --> 4'b = 0100
   PORTC = 0x00;	//initialize to 0
   unsigned char PIN_A = 0x00;

   

   while(1){
	   unsigned char cnt_used = 0x00;	//holds number of available spaces
	PIN_A = PINA;
	   //check each bit to count number of available spaces
	   if((PIN_A & 0x01) == 0x01){	//check first bit (index 0)
		   cnt_used++;
	   }
	   if((PIN_A & 0x02) == 0x02){	//checking second bit (index 1)
		   cnt_used++;
	   }
	   if ((PIN_A & 0x04) == 0x04){	//check third bit (index 2)
		   cnt_used++;
	   }
	   if ((PIN_A & 0x08) == 0x08){	//check fourth bit (index 3)
		   cnt_used++;
	   }

	   //output number of available spaces
	   PORTC = 0x04 - cnt_used;
   }
   return 0;
   }

