/*
 * EEPROM.c
 *
 * Created: 7/6/2014 1:06:30 PM
 * Author: Armia Wagdy
 */ 

#include <avr/io.h>
#include "EEPROM.h"

void eeprom_write(unsigned int address, unsigned char data)
{
	while((EECR & (1 << EEWE)) == 1);			//Wait for EEWE = 0, Which means that EEPROM is ready for read or write
	EEAR = address;							//Put the address in which you want to put the byte
	EEDR = data;							//Put the desired byte
	EECR |= (1 << EEMWE);					//Set EEMWE to 1
	EECR |= (1 << EEWE);					//Set EEWE to 1 within 4 clock cycles (EEMWE will be cleared after 4 clock cycles)
	_delay_ms(100);
}

unsigned char eeprom_read(unsigned char address)
{
	while((EECR & (1 << EEWE)) == 1);			//Wait for EEWE = 0, Which means that EEPROM is ready for read or write
	EEAR = address;							//Put the address of byte you want to read
	EECR |= (1 << EERE);					//Set EERE to 1
	return EEDR;							//Return the desired byte
}