/*
 * yfs401.c
 *
 * Created: 2022-05-02 16:05:33
 *  Author: ke5144ra-s
 */ 

#include "yfs401.h"

void flow_init(void) {
	GICR |= (1 << INT1);
	MCUCR |= (3 << ISC10);
}