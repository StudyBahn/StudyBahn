/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file init.c
	@brief Simple logic function with LEDs and Buttons
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";

#include <avr/io.h>
#include "init.h"

int main(void){


	init();

	while(1){
		if ((~PINB & (1 << PB0)))
            PORTB |= (1 << PB1);
		else
            PORTB &= ~(1 << PB1);

	}

	return 0;
}
