/*
 * mm74c922.h
 *
 * Created: 2022-05-02 09:52:03
 *  Author: ke5144ra-s
 */ 


#ifndef MM74C922_H_
#define MM74C922_H_

#include <avr/io.h>

#define KB_DATA		PINA
#define KB_A		(1 << PA1) // 0b00000010
#define KB_B		(1 << PA2) // 0b00000100
#define KB_C		(1 << PA3) // 0b00001000
#define KB_D		(1 << PA4) // 0b00010000
#define KB_AVAIL	(1 << PD2) // INT0

void kb_init(void);
uint8_t kb_get_value(void);

#endif /* MM74C922_H_ */