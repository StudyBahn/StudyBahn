/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file main.c
	@brief ENTER discription
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";

/******************************************************************************/
/* INCLUDED FILES                                                             */
/******************************************************************************/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
/******************************************************************************/


/******************************************************************************/

/* FUNCTIONS                                                                  */

/******************************************************************************/

/**
	@brief Init the microcontroller
*/
void init(void){

	DDRB |=  (1 << DDB0);		// PB0 output
    TCCR1B |= (1 << CS10);      // Set the prescaler to 1 (no Prescaling), which means f = 8MHz
}
/******************************************************************************/




/**
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void){

	// Init
	init();

	// Loop forever
	while (1)
    {
        // Since we want to generate a 328 Hz, TCNT1 = (8000000Hz/328Hz) * 0.5 (duty cycle) - 1 = 12194
        if(TCNT1 >= 12194)
        {
            PORTB ^= (1 << PB0);
            TCNT1 = 0;
        }
	}

	return 0;
}
/******************************************************************************/
