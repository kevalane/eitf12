/*
 * led.c
 *
 * Created: 2022-05-02 11:46:04
 *  Author: ke5144ra-s
 */ 

#include "led.h"

void led_init(void) {
	DDRA |= LED;
	on = 0;
}

void led_turn_on(void) {
	PORTA |= LED;
	on = 1;
}

void led_turn_off(void) {
	PORTA &= ~LED;
	on = 0;
}

void led_toggle(void) {
	if (on)
		led_turn_off();
	else
		led_turn_on();
}