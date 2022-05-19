/*
 * mcp7940m.h
 *
 * Created: 2022-04-28 15:39:06
 *  Author: ke5144ra-s
 */ 


#ifndef MCP7940M_H_
#define MCP7940M_H_

#include <avr/io.h>
#include "i2c.h"

#define HOUR_24		0
#define HOUR_12		1
#define SEC_REG		0x00
#define MIN_REG		0x01
#define HOUR_REG	0x02

uint8_t currentSeconds;
uint8_t currentMinutes;
uint8_t currentHours;

void rtc_read_clock(void);
void rtc_start_clock(void);
void rtc_set_clock_format(uint8_t format);
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
uint8_t serialize_seconds(int raw_seconds);
uint8_t serialize_minutes(int raw_minutes);
uint8_t serialize_hours(int raw_hours);
uint8_t deserialize_seconds(uint8_t seconds);
uint8_t deserialize_others(uint8_t m);
#endif /* MCP7940M_H_ */