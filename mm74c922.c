/*
 * mm74c922.c
 *
 * Created: 2022-05-02 09:51:24
 *  Author: ke5144ra-s
 */ 

#include "mm74c922.h"

uint8_t kb_get_value(void) {
	return ((KB_DATA & (KB_A | KB_B | KB_C | KB_D)) >> 1);
}
