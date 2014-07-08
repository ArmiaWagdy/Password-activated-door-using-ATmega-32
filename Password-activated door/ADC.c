/*
 * ADC.c
 *
 * Created: 11/19/2013 8:11:59 PM
 *  Author: A R M T
 *  Description: 10-bit ADC
				 8 analog input channels single ended.
				 Vref can be INTERNAL or AREF or AVCC
				 Prescaler can be 1 or 2 or 4 or 8 or 16 or 32 or 64 or 128
 */ 

#include <avr/io.h>
#include "ADC.h"

//****************************************************************************
//This function enables ADC module
void adc_init(unsigned char prescaler, unsigned char vref, unsigned char location)
{
	ADCSRA |= (1<<ADEN); //enable ADC
	adc_clkPrescaler(prescaler);
	adc_voltageReference(vref);
	location ? (ADMUX |= (1<<ADLAR)) : (ADMUX &= ~(1<<ADLAR)); // If true location is left-justified and vice versa
	return;
}
//****************************************************************************
//This function selects the conversion time of the ADC
//NB: for no prescaler enter "1"
void adc_clkPrescaler(unsigned char prescaler)
{
	ADCSRA &= (~(1<<ADPS0)) & (~(1<<ADPS1)) & (~(1<<ADPS2)); // clear ADPS2:ADPS0 for initialization
	
	switch(prescaler)
	{
		case 2:
			ADCSRA |= (1<<ADPS0);
			break;
		case 4:
			ADCSRA |= (1<<ADPS1);
			break;
		case 8:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1);
			break;
		case 16:
			ADCSRA |= (1<<ADPS2);
			break;
		case 32:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS2);
			break;
		case 64:
			ADCSRA |= (1<<ADPS1) | (1<<ADPS2);
			break;
		case 128:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
			break;
		default:
			ADCSRA |= 0x00;			//don't change any thing if no prescalar
	}
	return;
}
//****************************************************************************
//This function selects the voltage reference
void adc_voltageReference(unsigned char Vref)
{
	ADMUX &= (~(1<<REFS0)) & (~(1<<REFS1)); // clear REFS1:REFS0 for initialization
	
	switch(Vref)
	{
		case AREF:
			ADMUX |= 0x00;
			break;
		case AVCC:
			ADMUX |= (1<<REFS0);
			break;
		case INTERNAL:
			ADMUX |= (1<<REFS0) | (1<<REFS1);
			break;
		default:
			ADMUX |= 0x00;
	}
	return;
}
//****************************************************************************
//This function tells the ADC module to start conversion
void adc_inputChannel(unsigned char input_channel)
{
	ADMUX &= 0xC0; // clear MUX4:MUX0 and ADLAR bit for initialization
	
	switch(input_channel)
	{
		case ADC0:
			ADMUX |= 0x00;
			break;
		case ADC1:
			ADMUX |= (1<<MUX0);
			break;
		case ADC2:
			ADMUX |= (1<<MUX1);
			break;
		case ADC3:
			ADMUX |= (1<<MUX0) | (1<<MUX1);
			break;
		case ADC4:
			ADMUX |= (1<<MUX2);
			break;
		case ADC5:
			ADMUX |= (1<<MUX0) | (1<<MUX2);
			break;
		case ADC6:
			ADMUX |= (1<<MUX1) | (1<<MUX2);
			break;
		case ADC7:
			//ADMUX |= 0x07;
			ADMUX |= (1<<MUX0) | (1<<MUX1) | (1<<MUX2);
			break;
		default:
			ADMUX |= (0x1F & input_channel);
	}
	return;
}
//****************************************************************************
unsigned int adc_read(unsigned char input_channel)
{
	int analogReading = 0;
	unsigned char LSB;
	unsigned char MSB;
	adc_inputChannel(input_channel);
	ADCSRA |= (1<<ADSC);    //start conversion
	while((ADCSRA & (1<<ADIF)) == 0); //Wait until conversion is done
	LSB = ADCL;
	MSB = ADCH;
	analogReading |= MSB;
	analogReading <<= 8;
	analogReading |= LSB;
	return analogReading;	
}
