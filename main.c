/*
 * main.c
 * 
 * Medidor de Volume com Ultrasom (HCSRS04)
 *
 * Created: 17/08/2025 18:06:14
 * Author : Amauri Tuoni
 */ 

#include "avr_macro.h"    
#include <avr/io.h>
#include "avr_gpio.h"
#include "avr_lcd.h"
#include "ultrasound_sensor_hcsr04.h"
#include "avr_lcd.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#define COMPRIMENTO 16.00f   //comprimento do recipiente
#define LARGURA 10.00f       //largura do recipiente
#define ALTURA 9.50f         //altura do recipiente
#define ZONA_MORTA 2.750f    //zona morta do sensor HCRS04
#define AREA                COMPRIMENTO*LARGURA      //area do recipiente
#define VOLUME_CHEIO_L      AREA*ALTURA*0.001f       //volume com recipiente cheio em litros
#define BUZZER              PB1                      //Pino do Buzzer


//*********************************************************
//Rotina de Escrita no LCD (altura em cm e volume (litros)
//*********************************************************

void Escreve_LCD(float altura_cm, float volume_litros, _Bool *flag_volume)   
{
	char lcd_buffer[17];                //buffer para escrita no Lcd
	snprintf(lcd_buffer,sizeof(lcd_buffer),"Altura: %.2f cm ",altura_cm);   //info de altura no LCD
	LCD_String_xy(1,0,lcd_buffer);                                          //escreve no LCD
	//snprintf(lcd_buffer,sizeof(lcd_buffer),"V_cm3: %.2f",volume_cm3);
	//LCD_String_xy(2,0,lcd_buffer);
	snprintf(lcd_buffer,sizeof(lcd_buffer),"V: %.2f L",volume_litros);     //info de volume em litros no LCD
	LCD_String_xy(2,0,lcd_buffer);                                         //escreve no LCD
	if(volume_litros >= VOLUME_CHEIO_L)             //Recipiente cheio?
	{
		snprintf(lcd_buffer,sizeof(lcd_buffer),"-CHEIO");     //info de CHEIO no LCD
		LCD_String_xy(2,10,lcd_buffer);                 //escreve no LCD
		*flag_volume = true;                            //indica que o recipiente está cheio
	}
	else
	{
		snprintf(lcd_buffer,sizeof(lcd_buffer),"      ");       //apaga a info de CHEIO no LCD
		LCD_String_xy(2,10,lcd_buffer);                         //escreve no LCD
		*flag_volume = false;                          //indica que o recipiente está vazio
	}
}

//*****************************************
// Rotina de Acionamento do Buzzer
//*****************************************

void Buzzer_Sound(_Bool *flag_volume)
{
	static uint8_t buzzer_sound_count = 0;
	if(*flag_volume == true && buzzer_sound_count <= 2)
	{
		GPIO_WritePinHigh(BUZZER);
		_delay_ms(300);
		GPIO_WritePinLow(BUZZER);
		buzzer_sound_count++;
	}
	else if (*flag_volume == false)
	{
		GPIO_WritePinLow(BUZZER);
		buzzer_sound_count = 0;
	}
}

//*********************************************
//Programa Principal
//*********************************************
int main(void)
{
    float altura_cm = 0.0;              //altura medida
	float altura_anterior = 0.0;        //altura medida anteriormente
	float volume_cm3 = 0.0;             //volume em cm3
	float volume_litros = 0.0;          //volume em litros
	_Bool flag_volume_cheio = false;    //indica quando o recipiente está cheio
	
	HCSR04_Sensor_Setup();                //Rotina de configuração do Sensor
	GPIO_Init(PB,P1,GPIO_MODE_OUTPUT);    //configura PB1como saida
	GPIO_WritePinLow(BUZZER);             //Buzzer desligado
	LCD_Setup();                          //Rotina de configuração do LCD


    while (1)
	{ 
		HCSR04_Sensor_Trigger_Pulse();    //Envia o pulso para o sensor
		altura_anterior = altura_cm;      //Armazena a distancia medida anteriormente
		altura_cm = HCSR04_Sensor_Get_Distance();  //Captura a distancia atual
		altura_cm = (altura_cm + altura_anterior)/2;   //Calcula media da distancia atual e anterior
		
 		if((altura_cm-ZONA_MORTA) < ALTURA)     //A distancia (medida-zona morta do sensor) é menor que a distancia do recipiente? 
		{
			volume_cm3 = AREA*(altura_cm-ZONA_MORTA);    //calcula o volume em cm^3
			volume_litros = VOLUME_CHEIO_L - (volume_cm3*0.001f);   //calcula o volume em litros no recipiente
		}
		else             //maior ou igual?
		{
			altura_cm = 0.0;                
			volume_cm3 = 0.0;
			volume_litros = 0.0;  //recipiente vazio
		}
		Escreve_LCD(altura_cm,volume_litros, &flag_volume_cheio);  //escreve dados no LCD
		Buzzer_Sound(&flag_volume_cheio);                       //aciona o buzzer se está cheio

		_delay_ms(500);   //Base de tempo 500ms
	}
	
}

