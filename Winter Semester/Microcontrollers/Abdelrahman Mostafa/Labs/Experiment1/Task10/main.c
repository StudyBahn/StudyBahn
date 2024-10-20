/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file init.c
	@brief Simple logic function with LEDs and Buttons
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "init.h"

int main(void){

    // After Editing init.c file.
	init();

	while(1){


		if ((~PINB & (1 << PB0)) ){ // If Button at B0 is pressed:
			PORTB ^= (1 << PB1); // LED at B1 turns on

            _delay_ms(70); // Debouncing

            while(~PINB & (1<<PB0)); // Wait Until the button is pressed down:
            _delay_ms(70); // Debouncing
		}


	}

	return 0;
}
