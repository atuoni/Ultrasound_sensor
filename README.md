# Ultrasound sensor with AVR MCU

## About
This project measuring the volume of a container using an ultrasound sensor with the Arduino (ATMEGA328P). 
The project uses an HC-SR04 ultrasound sensor and code written in C language.


## How it works
The code was written in C language for the Atmega328P using Atmel Studio. A C language library was developed to use Timer 1 and the MCU's GPIOs.
Code was also written to use the HC-SR04 ultrasonic sensor by capturing Timer 1. The code also includes a library for using the 16x2 LCD display.

### Files:
- main.c     (test of the library)
- avr_macro.h  (header with macros)
- avr_gpio.c       (library for use of AVR Atmel GPIOs)
- avr_gpio.h       (header of the library AVR Atmel GPIOs)
- avr_lcd.c  (library for use LCD display)
- avr_lcd.h   (header of the library of LCD display)
- ultrasound_sensor_hcsr04.h
- ultrasound_sensor_hcsr04.c



## DEMONSTRATION VIDEO

[![Watch the video.](https://i9.ytimg.com/vi_webp/j767Vno2sWc/mq1.webp?sqp=CNj8xMcG&rs=AOn4CLA35gouhf2GBEex7KSgODKhKKkQ0g)](https://youtu.be/j767Vno2sWc)
