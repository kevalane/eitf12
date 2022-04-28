/*
 * lcd.c
 *
 * Created: 2022-04-28 17:27:01
 *  Author: ke5144ra-s
 */ 

#include "lcd.h"

void LCD_Command(unsigned char cmnd) {
	LCD_Data_Port = cmnd;
	LCD_Command_Port1 &= ~(1 << RS); // rs=0 command register
	LCD_Command_Port2 &= ~(1 << RW); // rw=0 write operation
	LCD_Command_Port2 |= (1 << EN); // enable pulse
	_delay_us(1); // 1 microsecond delay
	LCD_Command_Port2 &= ~(1 << EN);
	_delay_ms(3);
}

void LCD_Char(unsigned char char_data) {
	LCD_Data_Port = char_data;
	LCD_Command_Port1 |= (1 << RS); // rs=1 data reg
	LCD_Command_Port2 &= ~(1 << RW); // rw=0 => write operation like before
	LCD_Command_Port2 |= (1 << EN); // enable pulse!
	_delay_us(1);
	LCD_Command_Port2 &= ~(1 << EN);
	_delay_ms(1);
}

void LCD_Init(void) {
	LCD_Data_Dir = 0xff; // make lcd data port dir writeable
	LCD_Command_Dir1 |= (1 << RS); // make rs w
	LCD_Command_Dir2 |= (1 << RW); // same for rw pin
	LCD_Command_Dir2 |= (1 << EN); // ... and for en pin
	_delay_ms(20); // lcd power on delay has to be longer than 15ms

	LCD_Command(0x38);
	LCD_Command(0x0c);
	LCD_Command(0x06);
	LCD_Command(0x01);
	LCD_Command(0x80);
}

void LCD_String(char *str) {
	int i;
	for (i = 0; str[i] != 0; i++) {
		LCD_Char(str[i]);
	}
}

void LCD_String_xy(char row, char pos, char* str) {
	if (row == 0 && pos < 16) {
		LCD_Command((pos & 0x0f)|0x80);
	} else if (row == 1 && pos < 16) {
		LCD_Command((pos & 0x0f)|0xc0);
	}
	LCD_String(str);
}

void LCD_Clear() {
	LCD_Command(0x01);
	LCD_Command(0x80);
}
