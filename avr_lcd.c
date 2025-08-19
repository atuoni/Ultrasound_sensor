/*
 * avr_lcd.c
 *
 * LCD Library for AVR_Atmel
 *
 * Created: 31/07/2025 15:16:02
 *  Author: Amauri Tuoni
 */ 

#include "/Lib_AVR/avr_macro.h"
#include "/Lib_AVR/avr_lcd.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "/Lib_AVR/avr_gpio.h"

static const uint8_t cedilha[]=
{
	0b01110,  //Ç
	0b10001,
	0b10000,
	0b10000,
	0b10101,
	0b01110,
    0b10000
};


static const unsigned char delta[]=
{
		0b00100, //Delta
		0b00100,
		0b01010,
		0b01010,
		0b10001,
		0b11111,
		0b00000
};	

static const uint8_t battery_graphic[]=
{
//  row1, row2,...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,  //Character 0 ---- Battery Level 0
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x01,  //Character 1 ---- Battery Level 1
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,  //Character 2 ---- Battery Level 2
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F,  //Character 3 ---- Battery Level 3
	0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,  //Character 4 ---- Battery Level 4
	0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,  //Character 5 ---- Battery Level 5
	0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,  //Character 6 ---- Battery Level 6
	0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,  //Character 7 ---- Battery Level 7
};
//******************************
// Enable Pulse Function
//******************************

void LCD_Enable_Pulse(void)
{
	GPIO_WritePinHigh(EN);
	_delay_us(1);
	GPIO_WritePinLow(EN);
}

//****************************************
//LCD Command Function
//****************************************
void LCD_Command(uint8_t data)
{
	volatile uint8_t *port;
	port = &PORTD;
	GPIO_WritePinLow(RS);
	*port = (*port & 0x0F) | (data & 0xF0); //nibble superior
	//LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0); //nibble superior
	LCD_Enable_Pulse();
	_delay_us(200);
	*port = (*port & 0x0F) | (data << 4); //nibble inferior
	//LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4); //nibble inferior
	LCD_Enable_Pulse();
	_delay_ms(2);
}

//**************************************
//LCD Data Function
//**************************************

void LCD_Data(uint8_t data)
{
	//volatile uint8_t *port;
	//port = PORTD;
	GPIO_WritePinHigh(RS);    
	//port = (*port & 0x0F) | (data & 0xF0);   //nibble superior
	*PD = (*PD & 0x0F) | (data & 0xF0);   //nibble superior
	LCD_Enable_Pulse();
	_delay_us(200);
	//port = (*port & 0x0F) | (data << 4);  //nibble inferior
	*PD = (*PD & 0x0F) | (data << 4); //nibble inferior
	LCD_Enable_Pulse();
	_delay_ms(2);
}

//**************************************
//Clear Display Screen Function
//************************************** 

void LCD_Clear_Screen(void)
{
	LCD_Command(DISPLAY_CLEAR);
	_delay_ms(2);
	LCD_Command(CURSOR_HOME);
}
//***********************************
// Send a Nibble Data Function
//***********************************
void LCD_Nibble(uint8_t data)
{
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4); //nibble inferior
	LCD_Enable_Pulse();
}

//*********************************
// CGRAM Character Upload Function
//************************************

void LCD_Special_Characters_Upload(const uint8_t character[])
{
	uint8_t position;
	uint8_t location;
	for(location=0; location<8;location++)
	{
		position = location * 8;
		LCD_Command(CGRAM_ADDRESS + position);
		for(uint8_t i=position;i<position+8;i++)
		{
			LCD_Data(character[i]);
			
		}
	}
	LCD_Command(DDRAM_ADDRESS);
	//LCD_Command(0x40);
	//for(uint8_t i=0;i<7;i++)
	//	LCD_Data(character[i]);
	//LCD_Data(0x00);
	//LCD_Command(0x80);
}

//************************************************
//LCD Setup and MCU Initialization Function
//************************************************
void LCD_Setup()
{
	GPIO_InitPin(RS,GPIO_MODE_OUTPUT);
	GPIO_InitPin(EN,GPIO_MODE_OUTPUT);
	GPIO_InitPin(DATA_4,GPIO_MODE_OUTPUT);
	GPIO_InitPin(DATA_5,GPIO_MODE_OUTPUT);
	GPIO_InitPin(DATA_6,GPIO_MODE_OUTPUT);
	GPIO_InitPin(DATA_7,GPIO_MODE_OUTPUT);
	
	_delay_ms(40);
	
	LCD_Nibble(0x03);
	_delay_ms(10);
	LCD_Nibble(0x03);
	_delay_us(500);
	LCD_Nibble(0x03);
	LCD_Command(CURSOR_HOME);
	LCD_Command(_4BITS_2LINES_CHAR_5X8);
	LCD_Command(DISPLAY_OFF);
	LCD_Command(DISPLAY_CLEAR);
	LCD_Command(MODE_SET_RIGHT_NO_SCROLL);
	LCD_Command(DISPLAY_CURSOR_OFF_BLINK_OFF);
	
	_delay_ms(10);
		
	LCD_Special_Characters_Upload(cedilha);
	
	_delay_ms(10);
	
	LCD_Clear_Screen();
	
}
//**************************************
//Send Cursor to home position Function
//*************************************** 
void LCD_Cursor_Home(void)
{
	LCD_Command(CURSOR_HOME);
	_delay_ms(2);
}

//*************************************
//Function to send a string
//************************************

void LCD_String(char *string)
{
	uint8_t i;
	for(i=0;string[i]!='\0';i++)
	{
		LCD_Data(string[i]);
	}
}
//********************************************************
//Function to send a string in a determined position
//*******************************************************
void LCD_String_xy(const uint8_t line, const uint8_t column, char *string)
{
	if(line == LCD_LINE_SIZE-1 && column < LCD_COLUMN_SIZE)
		LCD_Command((column & 0x0F)|FIRST_LINE_ADDRESS);
	else if (line == LCD_LINE_SIZE && column < LCD_COLUMN_SIZE)
		LCD_Command((column & 0x0F)|SECOND_LINE_ADDRESS);
	LCD_String(string);
}

//******************************************
// Send a integer number to LCD
//******************************************
void LCD_Integer_To_String(const uint8_t line, const uint8_t column, const uint32_t number)
{
	char string[LCD_BUFFER_SIZE];
	sprintf(string,"%lu",number);
	
	LCD_String_xy(line, column, string);
}
//*******************************************
//Send a float number to LCD
//*******************************************
void LCD_Float_To_String(const uint8_t line, const uint8_t column, const float number, const uint8_t precision)
{
	char string[LCD_BUFFER_SIZE];
	//sprintf(string,"%f",number);
	dtostrf(number,2,precision,string);
	LCD_String_xy(line,column,string);
}

//*****************************************
// Print a Special Character in LCD
//****************************************
void LCD_Special_Char_Print(const uint8_t character)
{
	LCD_Data(character);
}

//*************************************************
// Scroll the text in LCD in a dtermined direction
//**************************************************
void LCD_Scroll(const uint8_t direction)
{
	uint8_t column = 0;
	if (direction == LEFT)
	{
		do
		{
			LCD_Command(TEXT_SHIFT_LEFT);
			column++;
			_delay_ms(SCROLL_TIME_MS);
		}
		while(column<LCD_COLUMN_SIZE);
	}
	else if (direction == RIGHT)
	{
		do 
		{
			LCD_Command(TEXT_SHIFT_RIGHT);
			column++;
			_delay_ms(SCROLL_TIME_MS);
		}
		while(column<LCD_COLUMN_SIZE);
	}
}