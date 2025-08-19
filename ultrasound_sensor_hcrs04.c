/*
 * Library HCSR04 Ultrasound Sensor for Atmel AVR
 *
 * Created: 06/08/2025 13:04:01
 * Author : Amauri Tuoni
 */ 

#include "avr_macro.h"
#include "avr_gpio.h"
#include "avr_timer1.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ultrasound_sensor_hcsr04.h"

extern volatile uint8_t end_capture;
extern volatile uint32_t width_ticks;


void HCSR04_Sensor_Setup(void)
{
	  GPIO_Init(PB,P5,GPIO_MODE_OUTPUT);
	  GPIO_Init(PB,P0,GPIO_MODE_INPUT_WITHOUT_PULL_UP);
      timer1_Setup_Capture_Mode();
	  sei(); // Habilita interrupções globais (se precisar)
}

void HCSR04_Sensor_Trigger_Pulse(void)
{
	// Gera um pulso de 10us no pino TRIGGER
	GPIO_WritePinLow(TRIGGER_PIN); // Define o pino como LOW
	_delay_us(2);
	GPIO_WritePinHigh(TRIGGER_PIN);  // Define o pino como HIGH
	_delay_us(10);
	GPIO_WritePinLow(TRIGGER_PIN); // Define o pino como LOW

}


float HCSR04_Sensor_Get_Distance(void)
{
	float distance = 0.0;
	static uint32_t width_us;
	if(end_capture)
	{
		cli();
		width_us = ticks_to_us(width_ticks);
		distance = (float)width_us/58.0;
		end_capture = 0;
		sei();
	}
	return distance;
}