/*
 * menu.c
 *
 * Created: 2022-05-02 12:02:29
 *  Author: ke5144ra-s
 */ 

#include "menu.h"

void init_menu(void) {
	freePosition = 0;
	for (int i = 0; i < 8; i++) {
		input[i] = 0x0;
	}
}

void start_menu(void) {
	in_main_menu = 1;
	page = 0;
	LCD_Clear();
	LCD_String_xy(0, 0, "[1] - Pour 4cl shot");
	LCD_String_xy(1, 0, "[2] - Pour 6cl shot");
	LCD_String_xy(2, 0, "[3] - Pour 8cl shot");
	LCD_String_xy(3, 0, "[N] - Next -->");
}

void generate_page_one() {
	in_main_menu = 1;
	page = 1;
	LCD_Clear();
	LCD_String_xy(0, 0, "[4] - Pour XXcl shot");
	LCD_String_xy(1, 0, "[5] - Set Time");
	LCD_String_xy(2, 0, "[6] - Help");
	LCD_String_xy(3, 0, "[B] - <-- Back");
}

void change_page(uint8_t page) {
	switch(page) {
		case 0:
			in_main_menu = 1;
			start_menu();
			break;
		case 1:
			in_main_menu = 1;
			generate_page_one();
			break;
	}
}

void handle_key_pressed(uint8_t key) {

	if (pouring) return;
	switch (key) {
		case 0: // 1
			if (in_main_menu && page == 0) generate_pouring_page(4);
			if (page == 5 || page == 7) add_character_to_input('1');
			break;
		case 1: // 2
			if (in_main_menu && page == 0) generate_pouring_page(6); // pour shot
			if (page == 5 || page == 7) add_character_to_input('2');
			break;
		case 2: // 3
			if (in_main_menu && page == 0) generate_pouring_page(8); // pour shot
			if (page == 5 || page == 7) add_character_to_input('3');
			break;
		case 3: // N
			if (in_main_menu) change_page(1);
			break;
		case 4: // 4
			if (page == 5 || page == 7) add_character_to_input('4');
			if (in_main_menu && page == 1) generate_custom_pour(); // pour shot
			break;
		case 5: // 5
			if (page == 5 || page == 7) add_character_to_input('5');
			if (in_main_menu && page == 1) generate_time_page();
			if (in_main_menu) {}// pour shot
			break;
		case 6: // 6
			if (in_main_menu && page == 1) generate_help_page();
			if (page == 5 || page == 7) add_character_to_input('6');
			break;
		case 7: // Back
			if (in_main_menu) change_page(0);
			if (page == 2) change_page(1);
			if (page == 5) change_page(1);
			if (page == 7) change_page(1);
			if (page == 8) change_page(0);
			if (page == 6) change_page(1);
			init_menu();
			break;
		case 8: // 7
			if (in_main_menu) {}// pour shot
			if (page == 5 || page == 7) add_character_to_input('7');
			break;
		case 9: // 8
			if (in_main_menu) {}// pour shot
			if (page == 5 || page == 7) add_character_to_input('8');
			break;
		case 10: // 9
			if (in_main_menu) {}// pour shot
			if (page == 5 || page == 7) add_character_to_input('9');
			break;
		case 11: // ?
			break;
		case 12: // *
			break;
		case 13: // 0
			if (in_main_menu) {}// pour shot
			if (page == 5 || page == 7) add_character_to_input('0');
			break;
		case 14: // #
			break;
		case 15: // Enter
			done = 1;
			if (page == 5) set_time(); // done with time
			if (page == 7) pour_custom_shot();
			for (int i = 0; i < 8; i++) {
				input[i] = '-';
			}
			freePosition = 0;
			break;
		default:
			break;
	}
}

void set_not_pouring(void) {
	pouring = 0;
}

void generate_help_page(void) {
	in_main_menu = 0;
	page = 2;
	LCD_Clear();
	LCD_String_xy(0, 0, "github.com/kevalane");
	LCD_String_xy(1, 0, "/eitf12");
	LCD_String_xy(2, 0, "");
	LCD_String_xy(3, 0, "[B] <-- Back");
}

void generate_pouring_page(unsigned char cl) {
	rtc_read_clock();
	if (currentHours < 12 && currentHours >= 6) {
		uint8_t time_left = 12 - currentHours;
		page = 8;
		in_main_menu = 0;
		// gen text
		char str[3];
		str[0] = time_left + 48;
		str[1] = 'h';
		str[2] = 0;
		LCD_Clear();
		LCD_String_xy(0, 0, "You cannot drink now");
		LCD_String_xy(1, 0, "You have to wait:");
		LCD_String_xy(2, 5, str);
		LCD_String_xy(3, 0, "[B] <-- Back");
	} else {
		in_main_menu = 0;
		page = 3;
		LCD_Clear();
		LCD_String_xy(1, 0, "Pouring ...");
		cl_to_pour = cl;
		char str[5];
		str[0] = (char) ((int) (cl / 10)) + 48;
		str[1] = (char) (cl % 10) + 48;
		str[2] = 'c';
		str[3] = 'l';
		str[4] = 0;
		if (cl == 11) {
			str[0] = input[0];
			str[1] = input[1];
			if (input[0] == 48) {
				cl_to_pour = input[1] - 48;
			} else {
				cl_to_pour = (input[0] - 48)*10 + (input[1] - 48);			
			}
		}
		LCD_String_xy(2,0,str);
		open_valve_1();	
	}
}

void generate_finish_page(uint8_t cl) {
	in_main_menu = 0;
	page = 4;
	LCD_Clear();
	LCD_String_xy(1, 0, "Your drinkeroni is done");
}

void generate_time_page() {
	in_main_menu = 0;
	page = 5;
	LCD_Clear();
	LCD_String_xy(0, 0, "Set time: ");
	LCD_String_xy(1, 0, "__-__-__");
	LCD_String_xy(2, 0, "hh-mm-ss");
	LCD_String_xy(3, 0, "[B] <-- Back");
	done = 0;
}

void update_time_page() {
	if (page == 5) {
		//LCD_Clear();
		//LCD_String_xy(0, 0, "Set time: ");
		LCD_String_xy(1, 0, input);
		//LCD_String_xy(2, 0, "hh-mm-ss");
		//LCD_String_xy(3, 0, "[B] <-- Back");
	}
}

void add_character_to_input(char c) {
	if (freePosition < 8 && page == 5) {
		input[freePosition] = c;
		freePosition++;
		update_time_page();
		if (freePosition == 2 || freePosition == 5) {
			input[freePosition] = '-';
			freePosition++;
		}
	}
	
	if (page == 7 && freePosition < 2) {
		input[freePosition] = c;
		freePosition++;
		update_custom_pour_page();
		if (freePosition == 0) input[1] = '_';
	}
}

void set_time() {
	page = 6;
	// check input?
	uint8_t s;// = (uint8_t) (input[6] + input[7]);
	uint8_t m;// = (uint8_t) (input[3] + input[4]);
	uint8_t h;// = (uint8_t) (input[0] + input[1]);
	if (input[0] == 48) {
		h = input[1] - 48;
	} else {
		h = (input[0] - 48)*10 + (input[1] - 48);
	}
	if (input[3] == 48) {
		m = input[4] - 48;
	} else {
		m = (input[3] - 48)*10 + (input[4] - 48);
	}
	if (input[6] == 48) {
		s = input[7] - 48;
	} else {
		s = (input[6] - 48)*10 + (input[7] - 48);
	}
	currentHours = h;
	currentMinutes = m;
	currentSeconds = s;
	//((uint8_t) input[6]) * 10) + ((uint8_t) input[7]);
	rtc_set_time(h, m, s);
	LCD_Clear();
	LCD_String_xy(1, 0, "Time set!");
	LCD_String_xy(3, 0, "[B] <-- Back");
}

void generate_custom_pour() {
	in_main_menu = 0;
	page = 7;
	LCD_Clear();
	LCD_String_xy(0, 0, "Select shot size: ");
	LCD_String_xy(1, 0, "__");
	LCD_String_xy(2, 0, "XX cl");
	LCD_String_xy(3, 0, "[B] <-- Back");
}

void update_custom_pour_page() {
	if (page == 7) LCD_String_xy(1, 0, input);
}

void pour_custom_shot() {
	uint8_t cl; //(uint8_t) (input[0] + input[1] - 48 * 2);
	if (input[0] == 48) {
		// starts with 0
		cl = input[1] - 48;
	} else {
		//cl = concat(input[0], input[1]);// (input[0]*10 - 48 + input[1] - 48);
	}
	generate_pouring_page(11);
}