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
#include <avr/io.h>

/******************************************************************************/

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

/**
    @author Abdelrahman Mostafa
	@brief Init the microcontroller
*/
void init(void){
	// Digital I/O init----------------------------------------------------
	DDRB &= ~(1 << DDB0);		// PB0 as Input (key 1)
	PORTB |= ( 1 << PB0);		// Pullup PB0

	DDRB |=  (1 << DDB1);		// PB1 as Output (LED red)
	DDRB |=  (1 << DDB2);		// PB1 as Output (LED yellow)
	DDRB |=  (1 << DDB3);		// PB1 as Output (LED green)


	// ADC init------------------------------------------------------------
	DDRC &= ~(1 << DDC3);		// PC3 as Input (Poti)
	PORTC &= ~(1 << DDC3);		// Pullup PC3 OFF

	// ADMUX
	ADMUX = 0;
	ADMUX |= (1 << REFS0); // AVCC as reference
	ADMUX |= ((1<<MUX0) | (1<<MUX1));

	// ADCSRA
	ADCSRA = (1 << ADPS2)|(1 << ADPS1); // Set ADC Prescale to 64

	// Start conversion, Free running, Enalbe ADC
	ADCSRA |= (1 << ADSC)|(1 << ADATE)|(1 << ADEN);

}
/******************************************************************************/




/**
	@author Abdelrahman Mostafa
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void){

	// Init
	init();

	// Loop forever
	while (1){

		if (ADCW <= 340) // If volts less than or eqal 1.66V
            PORTB |= (1 << PB3); //Green LED at Port B3 turns on.
		else
            PORTB &= ~(1 << PB3); // Otherwise off.

        if ((ADCW > 340) && (ADCW <= 679)) // If volts maotr than 1.66V and less than or equal 3.32V
            PORTB |= (1 << PB2); // Yellow LED at Port B2 turns on.
		else
            PORTB &= ~(1 << PB2); // Otherwise off.

        if ((ADCW > 679) && (ADCW <= 1023)) // If volts More than 3.32V and less than or equal 5V.
            PORTB |= (1 << PB1); //Red LED at Port B1 turns on.
		else
            PORTB &= ~(1 << PB1); // Otherwise off.
	}

	return 0;
}
/******************************************************************************/
