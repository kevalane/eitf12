/*
 * drinkers.c
 *
 * Created: 2022-04-25 14:21:35
 * Author : ke5144ra-s
 */ 

#define F_CPU		8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <compat/twi.h>

#define LED			(1 << PA0)	// 0b00000001

// Keyboard
#define KB_DATA		PINA
#define KB_A		(1 << PA1) // 0b00000010
#define KB_B		(1 << PA2) // 0b00000100
#define KB_C		(1 << PA3) // 0b00001000
#define KB_D		(1 << PA4) // 0b00010000
#define KB_AVAIL	(1 << PD2) // INT0

// FOR OUR RTC
#define SCL_CLOCK	100000UL
#define HOUR_24		0
#define HOUR_12		1
#define SLA_R		0b11011111
#define SLA_W		0b11011110
#define SEC_REG		0x00
#define MIN_REG		0x01
#define HOUR_REG	0x02

void rtc_read_clock(void);
void rtc_start_clock(void);
void rtc_set_clock_format(uint8_t format);
void rtc_set_time(); // TODO
uint8_t serialize_seconds(int raw_seconds);
uint8_t serialize_minutes(int raw_minutes);
uint8_t serialize_hours(int raw_hours);

void twi_init(void);
unsigned char twi_start(unsigned char address);
void twi_start_wait(unsigned char address);
unsigned char twi_rep_start(unsigned char address);
void twi_stop(void);
unsigned char twi_write(unsigned char data);
unsigned char twi_read_ack(void);
unsigned char twi_read_nack(void);
uint8_t read_specific_address(uint8_t address);

volatile uint8_t kb_val;

ISR (INT0_vect)          //External interrupt_zero ISR
{
	kb_val =  ((KB_DATA & (KB_A | KB_B | KB_C | KB_D)) >> 1);
	switch (kb_val) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA |= LED;
	DDRA &= ~(KB_A | KB_B | KB_C | KB_D);
	DDRD &= ~KB_AVAIL;
	
	GICR |= (1 << INT0);
	MCUCR |= (3 << ISC00);
	
	twi_init(); // init twi
	int data;
	sei();
	
    while (1) 
    {
		PORTA |= LED;
		//_delay_ms(500);
		PORTA &= ~LED;
		//_delay_ms(500);
		//rtc_set_clock_format(HOUR_24);
		rtc_read_clock();
    }
}

void rtc_set_clock_format(uint8_t format) {
	uint8_t currentData = read_specific_address(HOUR_REG);
	twi_start(SLA_W);
	twi_write(HOUR_REG);
	uint8_t dataToSend = currentData;
	if (format) {
		// 12 hour
		dataToSend |= (HOUR_12 << 6);
	} else {
		// 24 hour
		dataToSend |= (HOUR_24 << 6);
	}
	twi_write(dataToSend);
	twi_stop();
}

void rtc_start_clock(void) {
	twi_start(SLA_W);
	twi_write(0x00);
	twi_write(0b10000000);
	twi_stop();
}

void rtc_read_clock(void) {
	int raw_seconds;
	int raw_minutes;
	int raw_hours;
	twi_start(SLA_W);
	twi_write(0x00);
	twi_start(SLA_R);
	raw_seconds = twi_read_ack();
	raw_minutes = twi_read_ack();
	raw_hours = twi_read_nack();
	uint8_t seconds = serialize_seconds(raw_seconds);
	uint8_t minutes = serialize_minutes(raw_minutes);
	uint8_t hours = serialize_hours(raw_hours);
	twi_stop();
}

uint8_t serialize_seconds(int raw_seconds) {
	uint8_t ones = (uint8_t) (raw_seconds & 0b00001111);
	uint8_t tens = (uint8_t) (raw_seconds & 0b01110000);
	tens = (tens >> 4);
	return ones + tens*10;
}

uint8_t serialize_minutes(int raw_minutes) {
	uint8_t ones = (uint8_t) (raw_minutes & 0b00001111);
	uint8_t tens = (uint8_t) (raw_minutes & 0b01110000);
	tens = (tens >> 4);
	return ones + tens*10;
}

uint8_t serialize_hours(int raw_hours) {
	uint8_t ones = (uint8_t) (raw_hours & 0b00001111);
	uint8_t tens = (uint8_t) (raw_hours & 0b00110000);
	tens = (tens >> 4);
	return ones + tens*10;
}

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