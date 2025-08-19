/*
 * Header da biblioteca de funcoes macro Atmel AVR
 *
 *
 * Created: 29/06/2025 23:46:12
 * Author : Amauri Tuoni
 */


#ifndef AVR_MACROS_H_
#define AVR_MACROS_H_

#pragma message ( "avr_macros.h included" )

#ifndef F_CPU
#define F_CPU     16000000UL
#endif

#define ENABLE  0x01
#define DISABLE 0x00

#define BUSY    2
#define ERROR   1
#define OK      0

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~(1<<bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  (1<<bit))

#define SETBIT(ADDRESS,BIT)   (ADDRESS |=  (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT)  (ADDRESS ^=  (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS  &  (1<<BIT))

#endif /* AVR_MACROS_H_ */
