 /*
 * Biblioteca do Timer 1 para Atmel AVR
 *
 * Created: 12/08/2025 23:46:12
 * Author : Amauri Tuoni
 */

#include "avr_macro.h"
#include "avr_timer1.h"
#include "avr_gpio.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


volatile uint32_t overflow_count = 0;
volatile uint32_t width_ticks = 0;
//volatile uint32_t period_ticks = 0;
volatile uint8_t end_capture = 0;


inline uint32_t ticks_to_us(uint32_t ticks)
{
	return (uint32_t)(((uint64_t)ticks*PRESCALER_TIMER1*1000000ULL)/F_CPU);	
}

static inline uint32_t compose_timestamp(void)
{
	uint32_t ovf = overflow_count;
	uint16_t icr = ICR1;
	if((TIFR1 & (1<<TOV1)) && icr < 0x8000)
		ovf++;
	
	return (ovf<<16)|icr;
}

void timer1_Setup_Normal_Mode(void)
{
	TCNT1 = 0x0000;   
	TCCR1A = (0<<COM1A0)|(0<<COM1A1);           //normal mode
	TCCR1A |= (0<<WGM10)|(0<<WGM11);
	TCCR1B = (0<<WGM12)|(0<<WGM13);
	switch(PRESCALER_TIMER1)
	{
		case 0:
			TCCR1B |= (0<<CS12)|(0<<CS11)|(0<<CS10);  //Timer0 stopped
			break;
		case 1:
			TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);  // No Prescaler  = 16MHz/(65536*1) = 244,15Hz   1/244,15 = 4ms
			break;
		case 8:
			TCCR1B |= (0<<CS12)|(1<<CS11)|(0<<CS10);  // CLK/8  = 16MHz/(65536*8) = 30,51Hz     1/30,51Hz = 32,8ms 
			break;
		case 64:
			TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);  // CLK/64  = 16MHz/(65536*64) = 3,81Hz    1/3,81Hz = 262ms
			break;
		case 256:
			TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);  // CLK/256 = 16MHz/(65536*256) = 0,95Hz   1/0,95Hz = 1s
			break;
		case 1024:
			TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);  // CLK/1024 = 16MHz/(65536*1024) = 0,23Hz     1/0,23 = 4,2s
			break;
		case 2:
			TCCR1B |= (1<<CS12)|(1<<CS11)|(0<<CS10);  //External clock T0 pin (Falling Edge)
			break;
		case 3:
			TCCR1B |= (1<<CS12)|(1<<CS11)|(1<<CS10);  //External clock T0 pin (Rising Edge)
			break;
		default:
			TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);  // No prescaler 
			break;
		
	}
	TIMSK1 = (1<<TOIE1);        //overflow interruption
}

void timer1_Setup_CTC_Mode(uint16_t compA_value, uint16_t compB_value)
{
	GPIO_Init(PB,P1,GPIO_MODE_OUTPUT);      // Set OC1A to output pin - PB1
	GPIO_Init(PB,P2,GPIO_MODE_OUTPUT);      // Set OC1B to output pin - PB2 
	TCNT1 = 0x0000;
	OCR1A = compA_value;  //comparation value A
	OCR1B = compB_value;  //comparation value B
	TCCR1A = (1<<COM1A0)|(0<<COM1A1)|(0<<WGM11)|(0<<WGM10);   //Set OC0A on compare match and CTC mode   
	TCCR1A |= (1<<COM1B0)|(0<<COM1B1);             //Set OC0B on compare match (now,turn it off)
	TCCR1B = (1<<WGM12)|(0<<WGM13); 
	switch (PRESCALER_TIMER1)
	{ 
		case 0:
		TCCR1B |= (0<<CS12)|(0<<CS11)|(0<<CS10);  //Timer0 stopped
		break;
		case 1:
		TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);  // No Prescaler
		break;
		case 8:
		TCCR1B |= (0<<CS12)|(1<<CS11)|(0<<CS10);  // CLK/8   
		break;
		case 64:
		TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);  // CLK/64
		break;
		case 256:
		TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);  // CLK/256
		break;
		case 1024:
		TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);  // CLK/1024
		break;
		case 2:
		TCCR1B |= (1<<CS12)|(1<<CS11)|(0<<CS10);  //External clock T0 pin (Falling Edge)
		break;
		case 3:
		TCCR1B |= (1<<CS12)|(1<<CS11)|(1<<CS10);  //External clock T0 pin (Rising Edge)
		break;
		default:
		TCCR1B = (0<<CS12)|(0<<CS11)|(1<<CS10);  // No prescaler
		break;
	}
	TIMSK1 = (0<<OCIE1A)|(1<<OCIE1B);  //interrupt enable CompA and/or CompB
}

void timer1_Setup_Capture_Mode(void)
{
	GPIO_Init(PB,P0,GPIO_MODE_INPUT_WITHOUT_PULL_UP);
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B |= (1<<ICES1);   //rising edge
	TCCR1B |= (1<<ICNC1);  
	switch (PRESCALER_TIMER1)
	{
		case 0:
		TCCR1B |= (0<<CS12)|(0<<CS11)|(0<<CS10);  //Timer1 stopped
		break;
		case 1:
		TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);  // No Prescaler
		break;
		case 8:
		TCCR1B |= (0<<CS12)|(1<<CS11)|(0<<CS10);  // CLK/8   
		break;
		case 64:
		TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);  // CLK/64
		break;
		case 256:
		TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);  // CLK/256
		break;
		case 1024:
		TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);  // CLK/1024
		break;
		case 2:
		TCCR1B |= (1<<CS12)|(1<<CS11)|(0<<CS10);  //External clock T0 pin (Falling Edge)
		break;
		case 3:
		TCCR1B |= (1<<CS12)|(1<<CS11)|(1<<CS10);  //External clock T0 pin (Rising Edge)
		break;
		default:
		TCCR1B |= (0<<CS12)|(0<<CS11)|(1<<CS10);  // No prescaler
		break;
	}	
	TIMSK1 |= (1<<ICIE1)|(1<<TOIE1);  //interrupt enable
}

ISR(TIMER1_OVF_vect)
{
//		if (overflow_count > 4)
//		{
//			overflow_count = 0;
//			GPIO_Toggle(PB,P5);  //Led Toggle in 32,8ms * 5 = 164ms
//		}
	overflow_count++;	
}

ISR(TIMER1_COMPA_vect)
{
	if(overflow_count >= 50)
	{
		overflow_count = 0;
//		GPIO_Toggle(PB,P5);  //Led Toggle in 32,8ms * 50 = 1,64s
	}
	overflow_count++;
}
ISR(TIMER1_COMPB_vect)
{
	if(overflow_count >= 50)
	{
		
		overflow_count = 0;
//		GPIO_Toggle(PB,P5); //Led Toggle in 32,8ms * 50 = 1,64s
	}	
	overflow_count++;
}

ISR(TIMER1_CAPT_vect)
{
	enum signal_edge {rising_edge,falling_edge};
	static uint32_t rising_time = 0;
	static uint8_t capture = rising_edge;
	
	uint32_t now = compose_timestamp();
	
	if(capture == rising_edge)
	{
		rising_time = now;
		FLIPBIT(TCCR1B,ICES1);  //prox capture descida
		capture = falling_edge;
		//GPIO_Toggle(PB,P5);
	
	}
	else
	{
		width_ticks = now - rising_time;
		//GPIO_Toggle(PB,P5);
		FLIPBIT(TCCR1B,ICES1);  //prox capture subida
		capture = rising_edge;
		end_capture = 1;
	}

}