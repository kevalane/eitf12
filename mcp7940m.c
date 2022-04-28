/*
 * mcp7940m.c
 *
 * Created: 2022-04-28 15:38:50
 *  Author: ke5144ra-s
 */ 

#include "mcp7940m.h"

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