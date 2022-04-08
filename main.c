#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#define F_CPU 16000000UL								//define microcontroller timer speed

#define Roller 0x01

 uint16_t half[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
void Init_IO(void) ;

int main(void) {
	uint8_t stepper_index = 0;

	Init_IO();
	
	while ((PINK & Roller) != 0) 
	{
		PORTC = half[stepper_index];
		_delay_ms(35);
		++stepper_index;
		stepper_index = stepper_index % 8;
	}
	
	while (1) {
		if ((PINA & 0x01) != 0) // pushbutton 1
		{
			while ((PINA & 0x01) != 0) {
				PORTC = half[stepper_index];
				_delay_ms(35);
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
				
				_delay_ms(35);

				--stepper_index;
			}
			// rotate counter clockwise
		}
	}
}

void Init_IO(void) {
	DDRK = 0x00;    // Set PORTK to input switch
	PORTK = Roller; // Set PORTK to roller switch

	DDRA = 0x00;
	PORTA = 0x03; // set PORTA 0 & 1 pushbuttons

	DDRC = 0x0F; // stepper motor output								change this back to 0x0F
	PORTC = 0x00;
}
