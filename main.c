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

// FOR OUR RTC
#define SLA_R   0b11010001                             //address for ds1307, MAYBE CHANGE
#define SLA_W   0b11010000
#define sec_reg 0x00
#define min_reg 0x01
#define hour_reg 0x02

void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_write(uint8_t data);
uint8_t twi_read_ack(void);
uint8_t twi_read_nack(void);
uint8_t twi_get_status(void);

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
	
	twi_init(); // init twi
	int data;
	sei();
	
    while (1) 
    {
		// let's read it
		//twi_start();
		data = read_specific_address(0);
		//twi_stop();
		
		PORTA |= LED;
		_delay_ms(500);
		PORTA &= ~LED;
		_delay_ms(500);
    }
}

void twi_init(void) {
	TWSR = 0x00; // removes all initial values (no status on init)
	/*
	SCL freq = CPU Clock freq/ (16 + 2*TWBR*4^TWPS)
	TWBR = Value of the TWI Bit Rate Register
	TWPS = Value of the prescaler bits in the TWI Status Register
	Sets scl to 400kHz?
	200kHz
	*/
	TWBR = 0x0c; // adds 0b00001100
	TWCR = (1 << TWEN); // activates the twi interface by setting 2nd bit
}

void twi_start(void) {
	TWCR |= (1 << TWINT); // set interrupt flag
	TWCR |= (1 << TWSTA); // become master
	TWCR |= (1 << TWEN); // like in init, starts interface
	while ((TWCR & (1 << TWINT)) == 0) {}; // if TWINT is not set, get stuck
}

void twi_stop(void) {
	TWCR |= (1 << TWINT); // we are done, interrupt flag
	TWCR |= (1 << TWSTO); // stop bit generates stop
	TWCR |= (1 << TWEN); // activates interface like before
}

void twi_write(uint8_t data) {
	TWDR = data; // data registry containing next byte to transmit, obvious
	TWCR |= (1 << TWINT); // interrupt flag that we are done
	TWCR |= (1 << TWEN); // reactivates interface
	while((TWCR & (1 << TWINT)) == 0) {}; // like before, if TWINT is not set, get stuck because it means we are not done
}

uint8_t twi_read_ack(void) {
	TWCR |= (1 << TWINT); // interrupt flag, we are done
	TWCR |= (1 << TWEN); // enable again
	TWCR |= (1 << TWEA); // setting to one generates an ACK pulse on twi bus
	while ((TWCR & (1 << TWINT)) == 0) {}; // wait for interrupt!
	return TWDR; // return the data on twi bus (is this the ACK?)	
}

uint8_t twi_read_nack(void) {
	TWCR |= (1 << TWINT); // interrupt flag, done
	TWCR |= (1 << TWEN); // enable again
	while ((TWCR & (1 << TWINT)) == 0) {}; // wait for interrupt to be set
	return TWDR; // have not generated ack, maybe this is empty now?
}

uint8_t twi_get_status(void) {
	uint8_t status;
	status = TWSR & 0b11111000; // we don't want the reserved bits, only bits 7-3!!
	return status;
}

int read_rtc(void) {
	int recv_data;
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	recv_data=TWDR;	
	return recv_data;
}

void twi_send_address_write() {
	TWDR = SLA_W;
	TWCR = (1<<TWINT) | (1<<TWEN);     // Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
	while (!(TWCR & (1<<TWINT)));      //Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received
	//return 0;
}

void twi_send_address_data(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	//return 0;
}

void twi_send_address_read() {
	TWDR = SLA_R;
	TWCR = (1<<TWINT) | (1<<TWEN); 
	while (!(TWCR & (1<<TWINT)));
	//return 0;
}

int read_specific_address(uint8_t address) {
	int returnData;
	twi_init();
	twi_start();
	twi_send_address_write();
	twi_send_address_data(address);
	twi_start();
	twi_send_address_read();
	returnData = read_rtc();
	twi_stop();
	return returnData;
}
