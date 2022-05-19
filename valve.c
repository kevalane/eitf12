/*
 * valve.c
 *
 * Created: 2022-05-03 13:51:26
 *  Author: ke5144ra-s
 */ 

#include "valve.h"

void init_valve(void) {
	DDRD |= VALVE1_PIN;
	DDRD |= VALVE2_PIN;
	open_1 = 0;
}

void open_valve_1(void) {
	PORTD |= VALVE1_PIN;
	open_1 = 1;
}

void close_valve_1(void) {
	PORTD &= ~VALVE1_PIN;
	open_1 = 0;
}

void toggle_valve_1(void) {
	if (open_1) close_valve_1();
	if (!open_1) open_valve_1();
}

void open_valve_2(void) {
	PORTD |= VALVE2_PIN;
	open_2 = 1;
}

void close_valve_2(void) {
	PORTD &= ~VALVE2_PIN;
	open_2 = 0;
}

void toggle_valve_2(void) {
	if (open_2) close_valve_2();
	if (!open_2) open_valve_2();
}