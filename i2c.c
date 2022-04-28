/*
 * i2c.c
 *
 * Created: 2022-04-28 15:40:32
 *  Author: ke5144ra-s
 */ 

#include "i2c.h"

void twi_init(void) {
	TWSR = 0;
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;
}

unsigned char twi_start(unsigned char address) {
	uint8_t twst;

	TWCR = 0;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // set interrupt flag - become master - like in init, starts interface
	while (!(TWCR & (1<<TWINT))); // if TWINT is not set, get stuck
	twst = TW_STATUS & 0xf8;
	if ((twst != TW_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);

	// wait for transmission
	while (!(TWCR & (1 << TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;
}

unsigned char twi_rep_start(unsigned char address) {
	return twi_start(address);
}

void twi_stop(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);  // we are done, interrupt flag, stop bit generates stop, activates interface like before
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));
}

unsigned char twi_write(unsigned char data) {
	uint8_t twst;
	TWDR = data; // data registry containing next byte to transmit, obvious
	TWCR = (1<<TWINT) | (1<<TWEN); // interrupt flag that we are done -  reactivates interface
	while(!(TWCR & (1<<TWINT))); // like before, if TWINT is not set, get stuck because it means we are not done
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;
}

unsigned char twi_read_ack(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // interrupt flag, we are done - enable again - setting to one generates an ack pulse on twi bus
	while(!(TWCR & (1<<TWINT))); // wait for interrupt!
	return TWDR; // return the data on twi bus (is this the ACK?)
}

unsigned char twi_read_nack(void) {
	TWCR = (1<<TWINT) | (1<<TWEN);// interrupt flag, done - enable again
	while(!(TWCR & (1<<TWINT)));// wait for interrupt to be set
	return TWDR;// have not generated ack, maybe this is empty now?
}

uint8_t read_specific_address(uint8_t address) {
	uint8_t data;
	twi_start(SLA_W);
	twi_write(address);
	twi_start(SLA_R);
	data = twi_read_nack();
	twi_stop();
	return data;
}