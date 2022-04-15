/*
 * servomotortest2.c
 *
 * Created: 4/12/2022 2:55:19 PM
 * Author : czhengli, ssiripor
 
 IO connections
 PORTF (ADC) PF0 and PF1 to potentiometers
 PB5 for OC1A
 PB6 for OC1B
 PK0 for Roller
 PA0&1 for pushbuttons
 PC0:4 for stepper motor 
 
 
 
 
 
 */ 

 #define F_CPU 16000000UL
  #define Roller 0x01
 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>		//allow interrupts
 
 


 uint16_t half[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};

//global variables 
volatile uint16_t ADC_value;
volatile uint16_t ADC_res0;
volatile uint16_t ADC_res1;
volatile uint8_t ADC_channel;

//prototype
void init_ADC(void);
void Timer_Init(void);
void Init_IO(void) ;
void stepper_home(void);


uint8_t stepper_index = 0; //variable 


int main(void)
{
	Init_IO();
	Timer_Init();
	init_ADC();
	sei();
	
	stepper_home();				//return to home position (stepper)
	
	
	while(1)
	{
		/*//Plunger servo
		OCR1A= 2200;		//extended for arm 
		_delay_ms(1000);
		OCR1A=3800;			//retracted for arm
		_delay_ms(1000);
		
		//lift servo
		OCR1B= 2400;		 //up full
		_delay_ms(1500);

		OCR1B=3650;			 //down full
		_delay_ms(1500);*/
		
		
		if(ADC_channel == 0)										//when channel one is selected, plunger
		{
			
			OCR1A = ((uint32_t)ADC_res0*25/16) + 2200;				//assign value to servo 1
		}
		if (ADC_channel == 1)										//when channel 2 is selected, arm
		{
			OCR1B = ((uint32_t)ADC_res1*625/512) + 2400;				//assign value to servo 2 
		}
		
		
		if ((PINA & 0x01) != 0) // pushbutton 1
		{
			while ((PINA & 0x01) != 0) {
				PORTC = half[stepper_index];
				_delay_ms(20);
				++stepper_index;
				stepper_index = stepper_index % 8;
			}
		}

		if ((PINA & 0x02) != 0) // pushbutton 2
		{
			while ((PINA & 0x02) != 0) {
				
				if (stepper_index == 0) {
					stepper_index = 7;
				}
				PORTC = half[stepper_index];
				
				_delay_ms(20);

				--stepper_index;
			}
			// rotate counter clockwise
		}
		
	}
}

void init_ADC(void)
{
	
	DDRF = 0x00;	//set PORTF as input
	PORTF = 0xFE;	//turn off pull-up resistor for LSB
	
	ADMUX = (1 << REFS0);
	//ref voltage: AVCC, default channel: 0, left align
	ADCSRA = (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADIE);	//prescaler: 128
	ADCSRA |= (1 << ADEN) | (1 << ADSC);	// enable conversion
}

ISR(ADC_vect)		//interrupt for ADC
{
	ADC_value = ADCL;						//sets ADC_value to ADCL
	ADC_value = ADCL | (ADCH << 8);			//adds last two bits
	
	if (ADC_channel == 0)
	{
		ADC_res0 = ADC_value;
	}
	else if (ADC_channel == 1)
	{
		ADC_res1 = ADC_value;
	}
	
	ADC_channel++;
	ADC_channel = ADC_channel % 2;			//alternates ADC_channel 
	
	ADMUX = (ADMUX & 0xE0) | ADC_channel;	//set ADC port
	ADCSRA |= (1 << ADSC);
}


void Timer_Init(void)
{
		//FOR TIMER1
		TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);		//NON Inverted PWM
		TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11);		//PRESCALER=8 MODE 14(FAST PWM)
		ICR1 = 20000;									//freq=50Hz
		DDRB |= (1<<PB5)|(1<<PB6);   //PWM Pins as Output
		
		
		
		//FOR TIMER3
// 		TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<WGM31);		//NON Inverted PWM
// 		TCCR3B |= (1<<WGM33)|(1<<WGM32)|(1<<CS31);		//PRESCALER=8 MODE 14(FAST PWM)
// 		ICR1 = 20000;									//freq=50Hz
// 		DDRE |= (1<<PE3);   //PWM Pins as Output
// 	
}


void Init_IO(void) {
	DDRK = 0x00;    // Set PORTK to input switch
	PORTK = Roller; // Set PORTK to roller switch

	DDRA = 0x00;
	PORTA = 0x03; // set PORTA 0 & 1 pushbuttons

	DDRC = 0x0F; // stepper motor output								change this back to 0x0F
	PORTC = 0x00;
}


void stepper_home(void)
{
	while ((PINK & Roller) != 0)
	{
		PORTC = half[stepper_index];
		_delay_ms(20);
		++stepper_index;
		stepper_index = stepper_index % 8;
	}
}
