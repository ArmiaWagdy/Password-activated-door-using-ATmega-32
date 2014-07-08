/*
 * external_interrupts.c
 *
 * Created: 7/8/2014 7:45:46 AM
 *  Author: A R M T
 */ 

#include <avr/io.h>
#include "external_interrupts.h"

void set_externalInterrupt(unsigned char interrupt_num, unsigned char state)
{
	switch(interrupt_num)
	{
		case INT0:
			if (state == LOW_LEVEL)
				MCUCR = 0x00;
			else if (state == CHANGE)
				MCUCR |= (1 << ISC00);
			else if (state == FALLING_EDGE)
				MCUCR |= (1 << ISC01);
			else if (state == RISING_EDGE)	
				MCUCR |= ((1 << ISC00) | (1 << ISC01));	
			GICR |= (1 << INT0);
		break;
		
		case INT1:
			if (state == LOW_LEVEL)
				MCUCR = 0x00;
			else if (state == CHANGE)
				MCUCR |= (1 << ISC10);
			else if (state == FALLING_EDGE)
				MCUCR |= (1 << ISC11);
			else if (state == RISING_EDGE)
				MCUCR |= (1 << ISC10) | (1 << ISC11);
			GICR |= (1 << INT1);
		break;
		
		case INT2:
			if (state == FALLING_EDGE)
				MCUCR &= ~(1 << ISC2);
			else if (state == RISING_EDGE)
				MCUCR |= (1 << ISC2);
			GICR |= (1 << INT2);
		break;
		
		//default:
	}
}