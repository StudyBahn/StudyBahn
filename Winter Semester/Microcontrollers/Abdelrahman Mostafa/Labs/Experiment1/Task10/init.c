/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file init.c
	@brief Lab1 init inputs and outputs
*/

/*
This file contains all init steps in one function.
To call this function just use init();
*/

#include <avr/io.h>
#include "init.h"

void init(void){

	DDRB &= ~(1 << DDB0); // set PD2 data direction to input (zero)

	PORTB |= (1 << PB0); // enable internal pullup resistor for PD2

	DDRB |= (1 << DDB1); // set PB1 data direction to output (one)


}
