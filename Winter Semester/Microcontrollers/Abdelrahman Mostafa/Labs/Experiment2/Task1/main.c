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
    //CLKPR  |= (CLKPS3);
    TCCR1B |= (1 << CS10)|(1 << CS11);      // Set the prescaler to 64, which means f = 125000Hz
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
        if(TCNT1 >= 62499)
        {
            PORTB ^= (1 << PB0);
            TCNT1 = 0;
        }
	}

	return 0;
}
/******************************************************************************/
