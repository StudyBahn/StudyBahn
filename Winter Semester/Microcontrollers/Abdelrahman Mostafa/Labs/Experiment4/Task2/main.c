/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file main.c
	@brief Main programm for external DS1307
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";

/**
	@brief The CPU speed in Hz
*/
#define F_CPU 8000000UL

/**
	@brief I2C Address of the DS1307
*/
#define DS1307_I2C_ADR 0b1101000 // or in Hexadecimal = 0x68 is the Address of the DS1307 (Slave Address)

/******************************************************************************/
/* INCLUDED FILES                                                             */
/******************************************************************************/
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c_master.h"
#include "init.h"
#include "lcd.h"
/******************************************************************************/


/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

/**
	@brief Write a row byte to the DS1307
	@param adr address to write
	@param data byte to write
*/
void ds1307_write(uint8_t adr, char *data){

    i2c_master_open_write(DS1307_I2C_ADR);
    i2c_master_write(adr);
    while(*data!='\0')
    {
        i2c_master_write(*data); // It will be adding the string, char by char.
        data++;
    }
	i2c_master_close();
}
/******************************************************************************/
/**
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void){
	init(); 	// Function to initialise I/Os
	i2c_master_init(1, 10); // Init TWI
    char ID[7] = "SE29528";

    ds1307_write(0x08, ID);

    return 0;
    }

/******************************************************************************/
