/*
 * led.h
 *
 * Created: 2022-05-02 11:45:48
 *  Author: ke5144ra-s
 */ 


#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#define LED (1 << PA0)

uint8_t on;

void led_init(void);
void led_turn_on(void);
void led_turn_off(void);
void led_toggle(void);


#endif /* LED_H_ */