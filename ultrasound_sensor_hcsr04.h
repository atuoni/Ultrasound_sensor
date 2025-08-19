/*
 * Header HCSR04 Ultrasound sensor for AVR
 *
 * Created: 06/08/2025 13:04:01
 * Author : Amauri Tuoni
 */ 

#ifndef ULTRASOUND_SENSOR_HCSRO4_H_
#define ULTRASOUND_SENSOR_HCSR04_H_

#pragma message("ultrasound_sensor_hcsr04.h included")
 
 
#define TRIGGER_PIN PB5  // Pino de trigger do sensor
#define ECHO_PIN    PB0  // Pino de eco do sensor


//*******************************************
//Function Prototypes 
//*******************************************

void HCSR04_Sensor_Trigger_Pulse(void);
//uint32_t HCRS04_Sensor_Echo_Wait_Answer(void);
float HCSR04_Sensor_Get_Distance(void);
void HCSR04_Sensor_Setup(void);

#endif

