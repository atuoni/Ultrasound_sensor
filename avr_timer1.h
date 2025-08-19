/*
 * avr_timer1.h
 *
 * Library Header for Timer 1 for AVR MCU  
 *
 * Created: 12/08/2025 18:51:19
 *  Author: Amauri Tuoni
 */ 

#ifndef AVR_TIMER1_H_
#define AVR_TIMER1_H_

 #pragma message("avr_timer1.h included")
 
 #include <avr/io.h>
 
 #define PRESCALER_TIMER1    1     //TIMER0 Prescaler frequency
 #if PRESCALER_TIMER1 != 0 && PRESCALER_TIMER1 != 1 && PRESCALER_TIMER1 != 8 && PRESCALER_TIMER1 != 64 && PRESCALER_TIMER1 != 256 && PRESCALER_TIMER1 != 1024
	 && PRESCALER_TIMER1 != 2 && PRESCALER_TIMER1 != 3   //TIMER0 Extern Clock
 #error "Wrong prescaler definition for Timer 1!"
 #endif

  
 //******************************************
 //Prototypes
 //******************************************
 
 void timer1_Setup_Normal_Mode(void);
 void timer1_Setup_CTC_Mode(uint16_t compA_value, uint16_t compB_value);
 void timer1_Setup_Capture_Mode(void);
 uint32_t ticks_to_us(uint32_t ticks);
 

#endif /* AVR_TIMER1_H_ */