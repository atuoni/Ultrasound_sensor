# Volume Measure with AVR Microcontroller and HC-SR04 sensor

## About
This project measuring the volume of a container using an ultrasound sensor with the Arduino (ATMEGA328P). 
The project uses an HC-SR04 ultrasound sensor and code written in C language.


## How it works
The code was written in C language for the Atmega328P using Atmel Studio. A C language library was developed to use Timer 1 and the MCU's GPIOs.
Code was also written to use the HC-SR04 ultrasonic sensor by capturing Timer 1. The code also includes a library for using the 16x2 LCD display.

### Files:
- main.c     (main program)
- avr_macro.h  (header with macros)
- avr_gpio.c       (library for use of Atmel AVR GPIOs)
- avr_gpio.h       (Atmel AVR GPIOs library header)
- avr_lcd.c  (library for use LCD display)
- avr_lcd.h   (header of the library for LCD display)
- avr_timer1.c (library of the Atmel AVR Timer 1)
- avr_timer1.h (header library for Atmel AVR Timer 1)
- ultrasound_sensor_hcsr04.h  (header library for HCSR04 sensor)
- ultrasound_sensor_hcsr04.c  (library for use with HCSR04 sensor)



## DEMONSTRATION VIDEO

[![Watch the video.](https://i9.ytimg.com/vi_webp/j767Vno2sWc/mq1.webp?sqp=CNj8xMcG&rs=AOn4CLA35gouhf2GBEex7KSgODKhKKkQ0g)](https://youtu.be/j767Vno2sWc)
