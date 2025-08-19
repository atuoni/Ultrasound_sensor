/*
 * avr_lcd.h
 *
 * Header Library for LCD Display - Atmel AVR
 *
 * Created: 31/07/2025 13:19:28
 * Author : Amauri Tuoni
 */ 

#ifndef AVR_LCD_H_
#define AVR_LCD_H_

#pragma message ("avr_lcd.h included")

#include "avr/io.h"

#define LCD_4_BITS_MODE 

//#define PORT_LCD PORTD
//**************************************
// LCD Data Pins
//**************************************
#ifdef LCD_4_BITS_MODE
	#undef LCD_8_BITS_MODE 
	#define DATA_4  PD4
	#define DATA_5  PD5
	#define DATA_6  PD6
	#define DATA_7  PD7
	#define LCD_DATA_PORT   PORTD
#endif
#ifdef LCD_8_BITS_MODE
	#undef LCD_4_BITS_MODE 
	#define D0   
	#define D1
	#define D2
	#define D3
	#define D4
	#define D5
	#define D6
	#define D7
#endif
//**************************************
//LCD Control Pins
//*************************************
#define RS  PD2
#define EN  PD3

//***********************************
//LCD Size
//***********************************
#define LCD_LINE_SIZE     2
#define LCD_COLUMN_SIZE   16
#define DECIMAL_POINT     4
//***********************************
//LCD Commands
//***********************************
#define DISPLAY_CLEAR   0x01
#define CURSOR_HOME     0x02
#define DISPLAY_CURSOR_ON_BLINK_ON   0x0F
#define DISPLAY_CURSOR_ON_BLINK_OFF  0x0E 
#define DISPLAY_CURSOR_OFF_BLINK_ON  0x0D
#define DISPLAY_CURSOR_OFF_BLINK_OFF 0x0C
#define DISPLAY_OFF     0x08
#define DISPLAY_ON      0x0C
#define MODE_SET_LEFT_NO_SCROLL    0x04
//#define MODE_SET_LEFT_SCROLL       0x05
#define MODE_SET_RIGHT_NO_SCROLL   0x06
//#define MODE_SET_RIGHT_SCROLL      0x07
#define CURSOR_SHIFT_RIGHT   0x14  //desloca cursor para direita
#define CURSOR_SHIFT_LEFT    0x10    //desloca cursor para esquerda
#define TEXT_SHIFT_RIGHT  0x1C
#define TEXT_SHIFT_LEFT   0x18
#define _8BITS_2LINES_CHAR_5X8  0x38
#define _4BITS_2LINES_CHAR_5X8  0x28
#define _4BITS_1LINE_CHAR_5X8   0x30
#define _4BITS_1LINE_CHAR_5X11  0x34
#define DDRAM_ADDRESS      0x80
#define CGRAM_ADDRESS      0x40
#define FIRST_LINE_ADDRESS  0x80
#define SECOND_LINE_ADDRESS 0xC0
#define LEFT 0
#define RIGHT 1
#define SCROLL_TIME_MS 500
#define MAX_DIGITS 10

#define CGRAM_CHAR_0  0x00
#define CGRAM_CHAR_1  0x01
#define CGRAM_CHAR_2  0x02
#define CGRAM_CHAR_3  0x03
#define CGRAM_CHAR_4  0x04
#define CGRAM_CHAR_5  0x05
#define CGRAM_CHAR_6  0x06
#define CGRAM_CHAR_7  0x07

#define LCD_SYMBOL_DEGREE 0xDF
#define LCD_SYMBOL_OHM    0xF4
#define LCD_SYMBOL_PI     0xF8
#define LCD_SYMBOL_MICRO  0xE4
#define LCD_SYMBOL_BETA   0xE2
#define LCD_SYMBOL_RIGHT_ARROW 0x7E
#define LCD_SYMBOL_LEFT_ARROW 0x7F


//********************************
//Prototypes Functions
//********************************

void LCD_Clear_Screen(void);
void LCD_Setup(void);
void LCD_Cursor_Home(void);
void LCD_String(char *string);
void LCD_String_xy(const uint8_t line,const uint8_t column, char *string);
void LCD_Integer_To_String(const uint8_t line, const uint8_t column, const int number);
void LCD_Float_To_String(const uint8_t line, const uint8_t column, const float number,const uint8_t precision);
void LCD_Command(uint8_t data);  //apagar linha
//void LCD_Special_Characters_Upload(const uint8_t character[]);
void LCD_Data(uint8_t data);   //apagar linha
void LCD_Special_Char_Print(const uint8_t character);
void LCD_Scroll(const uint8_t direction);

#endif