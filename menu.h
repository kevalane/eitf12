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

#define ROW_1 0x80
#define ROW_2 0xc0
#define ROW_3 0x94
#define ROW_4 0xd4

uint8_t page;
uint8_t in_main_menu; // 1 if yes, 0 if no

void start_menu(void);
void change_page(uint8_t page);
void generate_page_one();
void handle_key_pressed(uint8_t key);

#endif /* MENU_H_ */