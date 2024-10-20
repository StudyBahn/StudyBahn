/**
	@author Abdelrahman Mostafa
	@author 29528
	@version 0.1
	@file one.c
	@brief Main programm for time clock
*/

const char MtrNum[] __attribute__((__progmem__)) = "29528";


/**
	@brief The CPU speed in Hz
*/
#define F_CPU 8000000UL

/**
	@brief I2C Address of the DS1307
*/
#define DS1307_I2C_ADR 0xD0 /*todo*/

/**
	@brief I2C Address of the EEPROM
*/
#define EEPROM_I2C_ADR 0xA0 /*todo*/




/**
	@brief Testing mode (automatic time skips)
*/
#define TESTMODE




/******************************************************************************/
/* INCLUDED FILES                                                             */
/******************************************************************************/
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c_master.h"
#include "init.h"
#include "lcd.h"
#include "stdio.h"
/******************************************************************************/


/******************************************************************************/
/* GLOBAL MEMORY                                                              */
/******************************************************************************/

// Global Time memory
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t month;
uint8_t year;
uint8_t weekday;

/******************************************************************************/



/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

/**
	@brief Convert from BCD to Binary
	@param in BCD input (00-99)
	@return Binary output
*/
uint8_t ds1307_decodeBcd(uint8_t in){
	return ((in>>4)*10 + in%16);
}
/******************************************************************************/

/**
	@brief Convert from Binary to BCD
	@param in Binary input (0-99)
	@return BCD output
*/
uint8_t ds1307_encodeBcd(uint8_t in){
	return ((in / 10) << 4 ) | (in % 10);
}
/******************************************************************************/


/**
	@brief Show time/date with the LCD
*/
void display_standby(uint8_t present, uint8_t pos){
	char str[16];

	// Time and Year
	snprintf(str, 16, "%02d:%02d:%02d  %02d.%02d", hour, minute,
			second, day, month);

	lcd_clear();
	lcd_string(str);


	// Show present or absent
	if (present)
		snprintf(str, 16, "present %d", pos);
	else
		snprintf(str, 16, "absent %d", pos);

	lcd_setcursor(0,2);
	lcd_string(str);

	return;
}
/******************************************************************************/

/**
	@brief Write a row byte to the DS1307
	@param adr address to write
	@param data byte to write
*/
void ds1307_write(uint8_t adr, uint8_t data){

	if (i2c_master_open_write(DS1307_I2C_ADR))
		return;

	i2c_master_write(adr);
	i2c_master_write(data);

	i2c_master_close();
}
/******************************************************************************/

/**
	@brief Read a row byte from the DS1307
	@param adr address to read
	@return the received byte
*/
uint8_t ds1307_read(uint8_t adr){
	uint8_t ret;

	if (i2c_master_open_write(DS1307_I2C_ADR))
		return 0;

	i2c_master_write(adr);
	i2c_master_open_read(DS1307_I2C_ADR);
	ret = i2c_master_read_last();

	i2c_master_close();

	return ret;

}
/******************************************************************************/

/**
	@brief Start or freeze the clock of the DS1307
	@param run zero for stop, all other for run
*/
void ds1307_rtc(uint8_t run){

	uint8_t readout;

	// Read current value
	readout = ds1307_read(0x00);


	// Set CH bit
	if (run)
		readout &= ~(0x80);
	else
		readout |= 0x80;

	// Write value back
	ds1307_write(0x00, readout);
}
/******************************************************************************/

/**
	@brief Write the current time to the DS1307
	@return zero for no error, one for communication error
*/
uint8_t ds1307_setTime(void){
	uint8_t chbit = ds1307_read(0x00) & 0x80;

	// Open device for write
	if (i2c_master_open_write(DS1307_I2C_ADR))
		return 1;

	i2c_master_write(0x00);
	if (chbit)
		i2c_master_write(ds1307_encodeBcd(second) | 0x80);
	else
		i2c_master_write(ds1307_encodeBcd(second) & 0x7F);

	i2c_master_write(ds1307_encodeBcd(minute));
	i2c_master_write(ds1307_encodeBcd(hour));

	i2c_master_write(weekday);

	i2c_master_write(ds1307_encodeBcd(day));
	i2c_master_write(ds1307_encodeBcd(month));
	i2c_master_write(ds1307_encodeBcd(year));


	// Close I2C bus
	i2c_master_close();

	return 0;
}
/******************************************************************************/

/**
	@brief Get the current time from the DS1307
	@return zero for no error, one for communication error
*/
uint8_t ds1307_getTime(void){

	// Open device for write
	if (i2c_master_open_write(DS1307_I2C_ADR))
		return 1;

	// select reading position (0x00)
	i2c_master_write(0x00);

	// (Re-)Open device for read
	i2c_master_open_read(DS1307_I2C_ADR);

	// Read value
	second = ds1307_decodeBcd(i2c_master_read_next() & 0x7F);
	// TODO minute, hour, ...
	minute = ds1307_decodeBcd(i2c_master_read_next()); // Read minutes from register 2
    hour = ds1307_decodeBcd(i2c_master_read_next() & 0x3F); //Mask out 12 hours mode bit
    weekday = ds1307_decodeBcd(i2c_master_read_next()); // Read Weekday from register 4
	day = ds1307_decodeBcd(i2c_master_read_next()); // Read day from register 5
    month = ds1307_decodeBcd(i2c_master_read_next()); // Read month from register 6
    year = ds1307_decodeBcd(i2c_master_read_last()); // Read year from register 7

	// Close I2C bus
	i2c_master_close();

	return 0;
}
/******************************************************************************/


/**
	@brief Load 8 bit value from the EEPROM
	@return loaded value
*/

uint8_t load_value8bit(uint8_t pos){
	uint8_t value;

	i2c_master_open_write(EEPROM_I2C_ADR);
    i2c_master_write(pos);
    i2c_master_open_read(EEPROM_I2C_ADR);
    value = i2c_master_read_last();
    i2c_master_close();

	//display_showload();

	return value;
}
/******************************************************************************/


/**
	@brief Save a 8 bit value to the EEPROM
	@param tosave value to save
*/
void save_value8bit(uint8_t tosave, uint8_t pos){

	i2c_master_open_write(EEPROM_I2C_ADR); //Device address, open to write
    i2c_master_write(pos); //Set the position pointer of the device
    i2c_master_write(tosave); //Write a byte

	i2c_master_close();
	_delay_ms(10); // wait 10ms to make sure that data is written
}
/******************************************************************************/


/**
	@brief Reset memory. Set memory pos to zero and reset present flag
*/
void reset_memory(void){


	// Reset data (only DS1307, no need to reset the EEPROM data)
	//todo
	ds1307_write(0x08, 0); // Will represent the present flag, 0 means absent.
	ds1307_write(0x09, 0); // Will represent the num. of working days.

	// Show reset info
	lcd_clear();
	lcd_string("memory reset");
	_delay_ms(100);

	// Wait for buttons release
	while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
		_delay_ms(100);

	_delay_ms(1000);

}
/******************************************************************************/

/**
	@brief Read memory. Show all datasets step by step
*/
void read_data(void){
	char str1[16];
	char str2[16];
	uint8_t i;
	uint8_t days;
    uint8_t months;
    uint8_t hour_arr;
    uint8_t min_arr;
    uint8_t hour_l;
    uint8_t min_l;
    uint8_t p = ds1307_read(0x09); // The address of the last data in the EEPROM.

	// Check for no data
	if ( 0 == load_value8bit(0x00)/*todo*/){
		lcd_clear();
		lcd_string("no data");

		// Wait for buttons release
		while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
			_delay_ms(100);
		_delay_ms(1000);

		return;
	}


	lcd_clear();
	lcd_string("read data");

	// Wait for buttons release
	while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
		_delay_ms(100);
	_delay_ms(1000);

	// Show data sets
	for (i = p; i > 0/*todo*/;){
		/*todo*/
		days = load_value8bit(i-6);
		months = load_value8bit(i-5);
		hour_arr = load_value8bit(i-4);
		min_arr = load_value8bit(i-3);
		hour_l = load_value8bit(i-2);
		min_l = load_value8bit(i-1);

		while(!( ~PINB & (1 << PB0)) &&  !( ~PINB & (1 << PB1)))
		{
            snprintf(str1, 16, "%02d/%02d Ar: %02d:%02d", days, months,
                hour_arr, min_arr);
            snprintf(str2, 16, "Leave: %02d:%02d", hour_l, min_l);

            lcd_clear();
            lcd_string(str1);
            lcd_setcursor(0,2);
            lcd_string(str2);
            _delay_ms(100);
		}
		if(~PINB & (1 << PB0))
            {i-=6;
            while(~PINB & (1 << PB0))
                {_delay_ms(75);}
            }

        else if(~PINB & (1 << PB1))
            {i+=6;
            while(~PINB & (1 << PB1))
                {_delay_ms(75);}
            }
        if(i>p)
            {i = p;}
    }

	// Ask for data reset
	lcd_clear();
	lcd_string("Yes  Reset");
	lcd_setcursor(0,2);
	lcd_string("No   data?");
	_delay_ms(20);
		while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
		;
	_delay_ms(20);

	// Continue if button 2 is pressed
	while (PINB & (1 << PB1)){
		// Reset if button 1 is pressed
		if (~PINB & (1 << PB0)){
			reset_memory();
			return;
		}
	}
}


/**
	@brief react on button: Arrive
*/
void button_arrive(void){
	char str[16];
	//uint8_t count = ds1307_read(0x09);
	uint8_t p = ds1307_read(0x09);

	// Check if memory is full
	if (ds1307_read(0x09)/6 == 42 /*256/6 (6 are the num. of inputs*/)
        return;


	// Check if already present
	// todo
	if(ds1307_read(0x08) != 0)
	{
	    _delay_ms(200);
	    while(!( ~PINB & (1 << PB0)) &&  !( ~PINB & (1 << PB1)))
        {
            lcd_clear();
            lcd_string("Already Present");
            lcd_setcursor(0,2);
            lcd_string("Overwrite Again?");
            _delay_ms(100);
        }

		if(( ~PINB & (1 << PB0) ))
		{
			save_value8bit(hour, p+2);
			save_value8bit(minute, p+3);

			// Show saved time
			snprintf(str, 16, "%02d:%02d:%02d  20%02d", hour, minute,
					second, year);

			lcd_clear();
			lcd_string(str);
			lcd_setcursor(0,2);
			lcd_string("Overwriten!");
			_delay_ms(1000);

			while (~PINB & (1 << PB0))
                {_delay_ms(100);}
            return; // We don't want him to write the values again after finishing this statement.
		}
		else if(~PINB & (1 << PB1))
        {
            while (~PINB & (1 << PB1))
				{_delay_ms(100);}
            return;
        }
	}


	// Save current time for arrive
	// todo
	//p = count*6;
    save_value8bit(day, p);
	save_value8bit(month, p+1);
	save_value8bit(hour, p+2);
	save_value8bit(minute, p+3);

	// Show saved time
	snprintf(str, 16, "%02d:%02d:%02d  20%02d", hour, minute,
			second, year);

	lcd_clear();
	lcd_string(str);
	lcd_setcursor(0,2);
	lcd_string("arrive saved");
	_delay_ms(1000);

	// Set present flag
	ds1307_write(0x08, 1);

	// Wait for buttons release
	while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
		_delay_ms(100);
	_delay_ms(100);

#ifdef TESTMODE
    if(minute > 57)
    {minute -= 58; hour++;}
    if(hour > 23)
    {hour -= 24; day++;}
    if(day > 29)
    {day -= 29; month++;}
	minute += 2;
	hour += 8;

	ds1307_setTime();
#endif



}
/******************************************************************************/


/**
	@brief react on button: Leave
*/
void button_leave(void){

	char str[16];
	//uint8_t count = ds1307_read(0x09);
	uint8_t p = ds1307_read(0x09);

	// Check if memory is full
	if(ds1307_read(0x09)/6 == 42)
		return;

	// Check if already absent
	// todo
	if(ds1307_read(0x08) == 0)
	{
	    _delay_ms(200);
	    while(!( ~PINB & (1 << PB0)) &&  !( ~PINB & (1 << PB1)))
        {
            lcd_clear();
            lcd_string("Already Absent");
            lcd_setcursor(0,2);
            lcd_string("Overwrite Again?");
            _delay_ms(100);
	    }

		if(( ~PINB & (1 << PB1) ))
		{
		    //p = count*6;
			save_value8bit(hour, p+4);
			save_value8bit(minute, p+5);

			// Show saved time
			snprintf(str, 16, "%02d:%02d:%02d  20%02d", hour, minute,
					second, year);

			lcd_clear();
			lcd_string(str);
			lcd_setcursor(0,2);
			lcd_string("Overwriten!");
			_delay_ms(1000);

			while ( ~PINB & (1 << PB1))
				{_delay_ms(100);}

            return; // We don't want him to write the values again after finishing this statement.
		}
		else if(~PINB & (1 << PB0))
        {
            while ( ~PINB & (1 << PB0))
				{_delay_ms(100);}
            return;
        }
	}

	// Save current time for leave
	// todo
    save_value8bit(hour, p+4);
    save_value8bit(minute, p+5);

	// Show saved time
	snprintf(str, 16, "%02d:%02d:%02d  20%02d", hour, minute,
			second, year);

	lcd_clear();
	lcd_string(str);
	lcd_setcursor(0,2);
	lcd_string("leave saved");

	// reset present flag and count up memory position
	// todo
	ds1307_write(0x08, 0);
	ds1307_write(0x09, p+6);


	// Wait for buttons release
	while (( ~PINB & (1 << PB0) ) || ( ~PINB & (1 << PB1) ))
		_delay_ms(100);
	_delay_ms(1000);

#ifdef TESTMODE
    if(minute > 49)
    {minute -= 50; hour++;}
    if(hour > 23)
    {hour -= 24; day++;}
    if(month == 12 && day > 29)
    {day = 1; month=1; year++;}
    if(day > 29)
    {day = 1; month++;}
    if(weekday >= 6)
    {weekday = 0; day += 1;} // I made weekday = 0, because it will increase by one after a few lines. So I don't want to increment it twice.
    hour = 7;
    minute += p/6;
    day++;
    weekday++;

    ds1307_setTime();
#endif

}
/******************************************************************************/


/**
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void)
{


	DDRB |= (1 << DDB5);		// DEBUG

	init(); 	// Function to initialise I/Os
	lcd_init(); // Function to initialise LCD display
	i2c_master_init(1, 10); // Init TWI
	ds1307_rtc(1);
	//reset_memory();


	// Check for reset button combination at power up
#ifdef TESTMODE
	if (( ~PINB & (1 << PB0) ) && ( ~PINB & (1 << PB1) )){
		reset_memory();
	}
#endif

	// Loop forever
	for(;;){

		// Short delay
		_delay_ms(100);
		PORTB ^= ( 1 << PB5);

		// Load current time/date from DS1307
		ds1307_getTime();

		// Show current time/date
		display_standby( ds1307_read(0x08), ds1307_read(0x09)/6 /*todo*/ );

		// Buttons
		// todo
		_delay_ms(100);
        if ((~PINB & (1 << PB1)) && (~PINB & (1 << PB0))){
            read_data();}
        else if ((~PINB & (1 << PB0)) ){ // If Button at B0 is pressed:
			button_arrive();}
        else if ((~PINB & (1 << PB1))){ // If Button at B1 is pressed:
			button_leave();}


	}

	return 0;
}
/******************************************************************************/
