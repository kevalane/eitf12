/*
 * lcd.h
 *
 * Created: 2022-04-28 17:27:11
 *  Author: ke5144ra-s
 */ 


#ifndef LCD_H_
#define LCD_H_

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD_Data_Dir DDRB
#define LCD_Command_Dir1 DDRA
#define LCD_Command_Dir2 DDRC
#define LCD_Data_Port PORTB
#define LCD_Command_Port1 PORTA
#define LCD_Command_Port2 PORTC
#define RS PA7
#define RW PC7
#define EN PC6

void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char char_data);
void LCD_Init(void);
void LCD_String(char* str);
void LCD_String_xy(char row, char pos, char* str);
void LCD_Clear();

#endif /* LCD_H_ */