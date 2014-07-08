/*
 * external_interrupts.h
 *
 * Created: 7/8/2014 7:45:27 AM
 *  Author: A R M T
 */ 


#ifndef EXTERNAL_INTERRUPTS_H_
#define EXTERNAL_INTERRUPTS_H_

#define LOW_LEVEL		0
#define CHANGE			1	
#define RISING_EDGE		2
#define FALLING_EDGE	3

void set_externalInterrupt(unsigned char interrupt_num, unsigned char state);


#endif /* EXTERNAL_INTERRUPTS_H_ */