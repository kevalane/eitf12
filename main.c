/*
 * drinkers.c
 *
 * Created: 2022-04-25 14:21:35
 * Author : ke5144ra-s
 */ 

#define F_CPU		8000000UL
//#define F_CPU	16000000UL
#define SEC 30
#define MIN 52
#define HOUR 16

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "mcp7940m.h"
#include "lcd.h"
#include "mm74c922.h"
#include "led.h"
#include "valve.h"
#include "menu.h"

volatile uint8_t kb_val;
uint8_t prevState;
volatile int counter1;
volatile int counter2;

int main(void)
{
	init_interrupt();
	twi_init(); // init twi
	LCD_Init(); // init LCD
	led_init(); // init LED
	init_valve(); // init valve
	init_menu(); // init menu
	sei();
	
	start_menu();
	
	
    while (1) 
    {
    }
}

void init_interrupt() {
	PCICR |= (1<<PCIE3);
	PCMSK3 |= (1<<PCINT26);
	PCMSK3 |= (1<<PCINT27);
	PCMSK3 |= (1<<PCINT28);
	prevState = PIND;
}

ISR(PCINT3_vect) {
	// second flowmeter
	if (!(PIND & (1 << PCINT26)) == (prevState & (1 << PCINT26))) {
		kb_val = kb_get_value();
		handle_key_pressed(kb_val);
	}
	
	if (!(PIND & (1 << PCINT27)) == (prevState & (1 << PCINT27))) {
		counter1++;
		if (counter1 > (45*cl_to_pour - 50)) {
			close_valve_1();
			counter1 = 0;
			open_valve_2();
		}
	}
	
	if (!(PIND & (1 << PCINT28)) == (prevState & (1 << PCINT28))) {
		counter2++;
		if (counter2 > 240) {
			close_valve_2();
			counter2 = 0;
		}
	}
	
	prevState = PIND;
}



