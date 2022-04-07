/*
 * stepper_motor.h
 *
 * Created: 2/2/2022 11:21:27 PM
 *  Author: Leo Siripornpitak
 */ 


#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


//function prototype

void stepper_drive(char mode, uint8_t rev);			//for driving the motor in full revolutions
void stepper_position(char mode, uint16_t degree);	//for driveing the motor with defined angles

//define

#define wave_step	0x01	//wave step controlled by PB0
#define full_step	0x02	//full step controlled by PB1
#define half_step	0x04	//half step controlled by PB2
#define angle_full	0x08	//angle mode full step controlled by PB3

//global variable
extern uint8_t wave[4];		//array of bit patterns for each mode
extern uint8_t full[4];
extern uint8_t half[8];

#endif /* STEPPER_MOTOR_H_ */