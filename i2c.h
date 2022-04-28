/*
 * i2c.h
 *
 * Created: 2022-04-28 15:40:20
 *  Author: ke5144ra-s
 */ 


#ifndef I2C_H_
#define I2C_H_

#define F_CPU		8000000UL
#define SCL_CLOCK	100000UL
#define SLA_R		0b11011111
#define SLA_W		0b11011110

#include <avr/io.h>
#include <compat/twi.h>

void twi_init(void);
unsigned char twi_start(unsigned char address);
void twi_start_wait(unsigned char address);
unsigned char twi_rep_start(unsigned char address);
void twi_stop(void);
unsigned char twi_write(unsigned char data);
unsigned char twi_read_ack(void);
unsigned char twi_read_nack(void);
uint8_t read_specific_address(uint8_t address);


#endif /* I2C_H_ */