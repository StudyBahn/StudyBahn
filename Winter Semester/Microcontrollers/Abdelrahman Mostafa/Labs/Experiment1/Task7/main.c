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


	// Init START
	init();
	// Init END

	while(1){
        // RED light
        if (!(~PIND & (1 << PD2)) && !(~PIND & (1 << PD3))) // If neither the buttons, which are connected to PORT D2, & D3 are pressed.
            PORTB |= (1 << PB0);    // RED Light. which is connected to PORT B0 will turn on.
        else
            PORTB &= ~(1 << PB0);   // Otherwise, it will turn off.

        if ((~PIND & (1 << PD2)) && (~PIND & (1 << PD3))) // If Both buttons are pressed.
            PORTB |= (1 << PB1); // Yellow light, which is connected to PORT B1 turns on.
        else
            PORTB &= ~(1 << PB1); // Otherwise, it turns off.

		if (!(~PIND & (1 << PD2)) != !(~PIND & (1 << PD3))) // If either button 1 or button 2 is pressed.
			PORTB |= (1 << PB2); // Green LED turns on.
		else
			PORTB &= ~(1 << PB2); // Otherwise it turns off.
            }

	return 0;
}
