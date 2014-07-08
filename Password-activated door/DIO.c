/*
 * DIO.c
 *
 * Created: 7/8/2014 6:52:15 AM
 *  Author: Armia Wagdy
 */ 

#include <avr/io.h>
#include "DIO.h"


/*****************************************************************************/

//This function sets the pin_number to either OUTPUT or INPUT

void DIO_pinmode(unsigned char port_char, unsigned char pin_number, unsigned char state)
{
	switch (port_char)
	{
		case 'A':
		case 'a':
		if(state == INPUT)
			DDRA &= ~(1 << pin_number);
		else if (state == INPUT_PULLUP)
		{
			DDRA &= ~(1 << pin_number);
			PORTA |= (1 << pin_number);
		}			
		else if (state == OUTPUT)
			DDRA |= (1 << pin_number);
		break;

		case 'B':
		case 'b':
		if(state == INPUT)
			DDRB &= ~(1 << pin_number);
		else if (state == INPUT_PULLUP)
		{
			DDRB &= ~(1 << pin_number);
			PORTB |= (1 << pin_number);		
		}				
		else if (state == OUTPUT)
			DDRB |= (1 << pin_number);		
		break;
		
		case 'C':
		case 'c':
		if(state == INPUT)
			DDRC &= ~(1 << pin_number);
		else if (state == INPUT_PULLUP)
		{
			DDRC &= ~(1 << pin_number);
			PORTC |= (1 << pin_number);
		}			
		else if (state == OUTPUT)
			DDRC |= (1 << pin_number);
		break;
		
		case 'D':
		case 'd':
		if(state == INPUT)
			DDRD &= ~(1 << pin_number);
		else if (state == INPUT_PULLUP)
		{
			DDRD &= ~(1 << pin_number);
			PORTD |= (1 << pin_number);			
		}			
		else if (state == OUTPUT)
			DDRD |= (1 << pin_number);
		break;
		
		//default:
		//#error "ERROR";
	}	
}

/*****************************************************************************/

//This function writes on pin number either HIGH or LOW

void DIO_write(unsigned char port_char, unsigned char pin_number, unsigned char logic_level)
{
		switch (port_char)
		{
			case 'A':
			case 'a':
			if(logic_level == LOW)
				PORTA &= ~(1 << pin_number);
			else if (logic_level == HIGH)
				PORTA |= (1 << pin_number);
			break;

			case 'B':
			case 'b':
			if(logic_level == LOW)
				PORTB &= ~(1 << pin_number);
			else if (logic_level == HIGH)
				PORTB |= (1 << pin_number);
			break;

			case 'C':
			case 'c':
			if(logic_level == LOW)
				PORTC &= ~(1 << pin_number);
			else if (logic_level == HIGH)
				PORTC |= (1 << pin_number);
			break;
			
			case 'D':
			case 'd':
			if(logic_level == LOW)
				PORTD &= ~(1 << pin_number);
			else if (logic_level == HIGH)
				PORTD |= (1 << pin_number);
			break;
			
			//default:
			//#error "ERROR";
		}
}

/*****************************************************************************/