/*
 * drinkers.c
 *
 * Created: 2022-04-25 14:21:35
 * Author : ke5144ra-s
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED	(1 << PA0)	// 0b00000001

// Keyboard
#define KB_DATA		PINA
#define KB_A (1 << PA1) // 0b00000010
#define KB_B (1 << PA2) // 0b00000100
#define KB_C (1 << PA3) // 0b00001000
#define KB_D (1 << PA4) // 0b00010000
#define KB_AVAIL (1 << PD2)		// INT0

volatile uint8_t kb_val;

ISR (INT0_vect)          //External interrupt_zero ISR
{
	kb_val =  ((KB_DATA & (KB_A | KB_B | KB_C | KB_D)) >> 1);
	
}

int main(void)
{
	DDRA |= LED;
	DDRA &= ~(KB_A | KB_B | KB_C | KB_D);
	DDRD &= ~KB_AVAIL;
	
	GICR |= (1 << INT0);
	MCUCR |= (3 << ISC00);
	
	sei(); 
	
    while (1) 
    {
		PORTA |= LED;
		_delay_ms(500);
		PORTA &= ~LED;
		_delay_ms(500);
    }
}

