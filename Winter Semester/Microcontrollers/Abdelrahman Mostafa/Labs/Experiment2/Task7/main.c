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
#include <avr/interrupt.h>
/******************************************************************************/


/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

/**
	@brief Init the microcontroller
*/
void init(void){

	// Init key inputs
	DDRD &=  ~(1 << DDD2);		// PD2 input
	DDRD &=  ~(1 << DDD3);		// PD3 input
	PORTD |= (1 << PD2); // enable Pullup PD2
	PORTD |= (1 << PD3); // enable Pullup PD3

	// Init LED outputs
	DDRB |=  (1 << DDB0);		// PB0 output Red LED
	DDRB |=  (1 << DDB1);		// PB0 output Yellow LED
	DDRB |=  (1 << DDB2);		// PB0 output Green LED

	// Enable interrupts
	EIMSK |= (1<<INT0);
	EIMSK |= (1<<INT1);
	TIMSK1 |= (1<<TOIE1);

	// Enable Timer1
	TCCR1B |=(1<<CS12); // Prescaler of 256

}
/******************************************************************************/




/**
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void){

	// Init
	init();

	// Global interrupt enable
	sei();

	// Loop forever
	while (1){

        /*
      if(TCNT1 >= 15625)
        {
        PORTB ^= (1<<PB1); // The same functionality as delays, It makes the frequency 1Hz. But this code, the overflow interrupt will never interrupt.
        TCNT1 = 0;
        }
       */

        _delay_ms(250);
        PORTB ^= (1<<PB1);
        _delay_ms(250);

	}

	return 0;
}
/******************************************************************************/

/**
	@brief INT0 interrupt
*/
ISR(INT0_vect){
	PORTB |= (1<<PB0);
}
/******************************************************************************/

/**
	@brief INT1 interrupt
*/
ISR(INT1_vect){
	PORTB &= ~(1<<PB0);
}
/******************************************************************************/

/**
	@brief Timer1 overflow interrupt
*/
ISR(TIMER1_OVF_vect){
	PORTB ^= (1<<PB2);
	}
/******************************************************************************/
