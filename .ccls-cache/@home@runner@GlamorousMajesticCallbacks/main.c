#include <stdio.h>

#include <avr/io.h>

#define Roller 0x01

#define half [8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};

int main(void) {
  uint8_t stepper_index = 0;

  while (1) {
    if ((PINA & 0x01)==1) // pushbutton 1
    {
      while (PINA & 0x01) {
        PORTC = half[stepper_index];
        _delay_ms(6);
        ++stepper_index;
        stepper_index = stepper_index % 8;
      }
    }

    if ((PINA & 0x02)==1) // pushbutton 2
    {
      while (PINA & 0x02) {
        if (stepper_index == 0) {
          stepper_index = 7
        }

        PORTC = half[stepper_index];
        _delay_ms(6);

        stepper_index--;
      }
      // rotate counter clockwise
    }
    if ((PINA & 0x04)==1) // pushbutton 3 //
    {
      while (~(PORTK & Roller)) 
      {
        for (stepper_index = 0; stepper_index < 8; stepper_index++)   // iterate through the bit patterns
        {
          PORTK = half[stepper_index];     // set output to be the bit patterns in
                                           // the array
          _delay_ms(6);
        }
      }
    }
  }
}

void Init_IO(void) {
  DDRK = 0x00;    // Set PORTK to input switch
  PORTK = Roller; // Set PORTK to roller switch

  DDRA = 0x00;
  PORTA = 0x03; // set PORTA 0 & 1 pushbuttons

  DDRC = 0x0F; // stepper motor output
  PORTC = 0x00;
}