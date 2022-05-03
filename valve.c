/*
 * valve.c
 *
 * Created: 2022-05-03 13:51:26
 *  Author: ke5144ra-s
 */ 

#include "valve.h"

void init_valve(void) {
	DDRD |= VALVE_PIN;
	open = 0;
}

void open_valve(void) {
	PORTD |= VALVE_PIN;
	open = 1;
}

void close_valve(void) {
	PORTD &= ~VALVE_PIN;
	open = 0;
}

void toggle_valve(void) {
	if (open) close_valve();
	if (!open) open_valve();
}