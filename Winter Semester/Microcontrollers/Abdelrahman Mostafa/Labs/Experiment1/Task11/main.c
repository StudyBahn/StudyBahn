/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file main.c
	@brief Analog output via PWM
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "init.h"

int main(void){


	init();

	while(1){
		if ((~PIND & (1 << PD2)) ){
		    // _delay_ms(100);
			PORTB ^= (1 << PB1);
		} else {
			PORTB |= (1 << PB1);
		}
	}

	return 0;
}
