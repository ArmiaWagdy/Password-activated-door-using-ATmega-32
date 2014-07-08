/*
 * EEPROM.h
 *
 * Created: 7/6/2014 1:06:51 PM
 * Author: Armia Wagdy
 */ 

#ifndef EEPROM_H_
#define EEPROM_H_
#define F_CPU 1000000UL
#include <util/delay.h>

void eeprom_write(unsigned int address, unsigned char data);
unsigned char eeprom_read(unsigned char address);

#endif /* EEPROM_H_ */