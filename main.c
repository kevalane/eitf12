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
#include "yfs401.h"
#include "valve.h"

volatile uint8_t kb_val;
volatile int counter;

int main(void)
{
	twi_init(); // init twi
	LCD_Init(); // init LCD
	kb_init(); // init keyboard
	led_init(); // init LED
	flow_init(); // init flow interrupt pin
	init_valve(); // init valve
	sei();
	
	//counter = 0;
	start_menu();
	
	
    while (1) 
    {
		open_valve();
		_delay_ms(1000);
		close_valve();
		_delay_ms(1000);
		//rtc_set_clock_format(HOUR_24);
		rtc_start_clock();
		//LCD_String(counter);
    }
}

ISR(INT1_vect) {
	counter++;
	if (counter > 10) {
		close_valve();
	}
}

ISR (INT0_vect)          //External interrupt_zero ISR
{
	kb_val = kb_get_value();
	handle_key_pressed(kb_val);
}



