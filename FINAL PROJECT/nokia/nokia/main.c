/*
 * nokia.c
 *
 * Created: 5/28/2019 6:53:22 PM
 * Author : jenny
 */ 

#define F_CPU 8250000UL
#include <avr/io.h>
#include "nokia5110.h"
#include "nokia5110.c"
//#include "nokia5110_chars.h"



int main(void)
{
    /* Replace with your application code */
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_power(1);
	//nokia_lcd_set_cursor(3,3);
	nokia_lcd_write_string("hello", 1);
	nokia_lcd_render();
    while (1) 
    {
    }
}

