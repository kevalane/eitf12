/*
 * menu.c
 *
 * Created: 2022-05-02 12:02:29
 *  Author: ke5144ra-s
 */ 

#include "menu.h"

void start_menu(void) {
	in_main_menu = 1;
	page = 0;
	LCD_Clear();
	LCD_String_xy(0, 0, "[0] - Pour 4cl shot");
	LCD_String_xy(1, 0, "[1] - Pour 6cl shot");
	LCD_String_xy(2, 0, "[2] - Pour 8cl shot");
	LCD_String_xy(3, 0, "[3] - Next -->");
}

void generate_page_one() {
	in_main_menu = 1;
	page = 1;
	LCD_Clear();
	LCD_String_xy(0, 0, "[4] - Pour XXcl shot");
	LCD_String_xy(1, 0, "[5] - Set Time");
	LCD_String_xy(2, 0, "[6] - Help");
	LCD_String_xy(3, 0, "[7] - <-- Back");
}

void change_page(uint8_t page) {
	switch(page) {
		case 0:
			start_menu();
			break;
		case 1:
			generate_page_one();
			break;
	}
}

void handle_key_pressed(uint8_t key) {
	switch (key) {
		case 0:
			led_turn_on();
			break;
		case 1:
			led_turn_off();
			break;
		case 2:
			led_toggle();
			break;
		case 3:
			change_page(1);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			if (in_main_menu) change_page(0);
			//if (!in_main_menu)
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