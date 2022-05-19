/*
 * valve.h
 *
 * Created: 2022-05-03 13:51:45
 *  Author: ke5144ra-s
 */ 


#ifndef VALVE_H_
#define VALVE_H_

#include <avr/io.h>
#define VALVE1_PIN (1 << PD0)
#define VALVE2_PIN (1 << PD1)
uint8_t open_1;
uint8_t open_2;

void init_valve(void);
void open_valve(void);
void close_valve(void);
void toggle_valve(void);


#endif /* VALVE_H_ */