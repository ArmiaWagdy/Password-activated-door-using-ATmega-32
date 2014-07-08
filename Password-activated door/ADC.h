/*
 * ADC.h
 *
 * Created: 11/19/2013 8:52:41 PM
 *  Author: A R M T
 */ 


#ifndef ADC_H_
#define ADC_H_

#define AREF 0
#define AVCC 1
#define INTERNAL 2

#define ADC0 0 
#define ADC1 1 
#define ADC2 2 
#define ADC3 3 
#define ADC4 4 
#define ADC5 5 
#define ADC6 6
#define ADC7 7  
#define RIGHT 0
#define LEFT 1

void adc_init(unsigned char prescaler, unsigned char vref, unsigned char location);
void adc_clkPrescaler(unsigned char prescaler);
void adc_voltageReference(unsigned char Vref);
void adc_inputChannel(unsigned char input_channel);
unsigned int adc_read(unsigned char input_channel);

#endif /* ADC_H_ */