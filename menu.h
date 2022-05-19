/*
 * menu.h
 *
 * Created: 2022-05-02 12:02:43
 *  Author: ke5144ra-s
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "lcd.h"
#include <avr/io.h>
#include "led.h"
#include "mcp7940m.h"
#include <string.h>
#include "valve.h"

#define ROW_1 0x80
#define ROW_2 0xc0
#define ROW_3 0x94
#define ROW_4 0xd4

uint8_t page; // 0 is first menu, 1 second menu, 2 is help, 3 is pouring, 4 is finish
// 5 is time, 6 is done with setting time, 7 is custom pour, 8 is go home alcoholic
uint8_t in_main_menu; // 1 if yes, 0 if no
volatile uint8_t pouring;
volatile uint8_t done;
volatile char input[9];
volatile uint8_t freePosition;
uint8_t cl_to_pour;

void init_menu(void);
void start_menu(void);
void change_page(uint8_t page);
void generate_page_one();
void handle_key_pressed(uint8_t key);
void set_not_pouring(void);
void generate_help_page(void);
void generate_pouring_page(unsigned char cl);
void generate_finish_page(uint8_t cl);
void generate_time_page();
void update_time_page();
void add_character_to_input(char c);
void set_time();
void generate_custom_pour();
void update_custom_pour_page();
void pour_custom_shot();

#endif /* MENU_H_ */

