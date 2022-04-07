/*
  stepper_motor.c
 
  Created: 2/2/2022 11:19:49 PM
  Author: Leo Siripornpitak
  
  Description: 
  This file has functions containing the functions necessary to drive the stepper motor.
  It is part of a multi module project and is to be used in conjunction with stepper_motor.h.
  This file has two functions in it, stepper_drive has a mode and revolution parameters
  and it is used to drive the stepper motor in full revolutions. Another function is
  stepper_position which has the same parameters as previous one and is used to drive
  the motor to specific angle corresponding to the degree parameter.
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "stepper_motor.h"


uint8_t half[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08}; //bit patterns for half step mode


void stepper_drive(char mode, uint8_t rev)
{
		
	uint16_t step;
	
	step = 512 * rev ;//how many times the inner loop has to repeat until reaching preferred revolution
	
	switch(mode)
	{
		case 'W':
			for(uint16_t i = 0; i < step; i++)	//repeat the patterns until preferred rev is reached
			{
				for(uint8_t j = 0; j < 4; j++)	//iterate through the bit patterns
				{
					PORTK = wave[j];	//set output to be the bit patterns in the array
					_delay_ms(6);
				}
			}
		break;
		
		case 'F':
			for(uint16_t i = 0; i < step; i++)	//repeat the patterns until preferred rev is reached
			{
				for(uint8_t j = 0; j < 4; j++)	//iterate through the bit patterns
				{
					PORTK = full[j];	//set output to be the bit patterns in the array
					_delay_ms(4);
				}
			}
		break;
		
		case 'H':
			for(uint16_t i = 0; i < step; i++)	//repeat the patterns until preferred rev is reached
			{
				for(uint8_t j = 0; j < 8; j++)	//iterate through the bit patterns
				{
					PORTK = half[j];	//set output to be the bit patterns in the array
					_delay_ms(6);
				}
			}
			break;
		
	}
}

void stepper_position(char mode, uint16_t degree)
{
	uint16_t step_angle;
	
	step_angle = degree / 0.7;	//times that inner loop need to repeat to complete required angle
	
	switch(mode)
	{
		case 'W':
		for(uint16_t i = 0; i < step_angle; i++)	//repeat the patterns until preferred rev is reached
		{
			for(uint8_t j = 0; j < 4; j++)	//iterate through the bit patterns
			{
				PORTK = wave[j];	//set output to be the bit patterns in the array
				_delay_ms(6);
			}
		}
		break;
		
		case 'F':
		for(uint16_t i = 0; i < step_angle; i++)	//repeat the patterns until preferred rev is reached
		{
			for(uint8_t j = 0; j < 4; j++)	//iterate through the bit patterns
			{
				PORTK = full[j];	//set output to be the bit patterns in the array
				_delay_ms(4);
			}
		}
		break;
		
		case 'H':
		for(uint16_t i = 0; i < step_angle; i++)	//repeat the patterns until preferred rev is reached
		{
			for(uint8_t j = 0; j < 8; j++)	//iterate through the bit patterns
			{
				PORTK = half[j];	//set output to be the bit patterns in the array
				_delay_ms(6);
			}
		}
		break;
		
	}
}