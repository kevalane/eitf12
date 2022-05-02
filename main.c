/*
 * drinkers.c
 *
 * Created: 2022-04-25 14:21:35
 * Author : ke5144ra-s
 */ 

#define F_CPU		8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "mcp7940m.h"
#include "lcd.h"
#include "mm74c922.h"
#include "led.h"

volatile uint8_t kb_val;

int main(void)
{
	twi_init(); // init twi
	LCD_Init(); // init LCD
	kb_init(); // init keyboard
	led_init(); // init LED
	int data;
	sei();

	LCD_String("Hello");
	LCD_Command(0xc0);
	LCD_String("World");
	LCD_Command(0x94);
	LCD_String("on");
	LCD_Command(0xd4);
	LCD_String("4 rows!!!");
	
    while (1) 
    {
		//rtc_set_clock_format(HOUR_24);
		//rtc_start_clock();
		rtc_read_clock();
    }
}

ISR (INT0_vect)          //External interrupt_zero ISR
{
	kb_val = kb_get_value();
	switch (kb_val) {
		case 0:
			led_turn_on();
			break;
		case 1:
			led_turn_off();
			break;
		case 2:
			led_toggle();
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		default:
			break;
	}
}



