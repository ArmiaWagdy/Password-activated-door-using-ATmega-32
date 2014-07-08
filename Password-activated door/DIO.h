/*
 * DIO.h
 *
 * Created: 7/8/2014 6:52:37 AM
 *  Author: A R M T
 */ 


#ifndef DIO_H_
#define DIO_H_

#define	INPUT			 0
#define OUTPUT			 1
#define INPUT_PULLUP	 2
#define LOW				 0
#define HIGH			 1

void DIO_pinmode(unsigned char port_char, unsigned char pin_number, unsigned char state);
void DIO_write(unsigned char port_char, unsigned char pin_number, unsigned char logic_level);



#endif /* DIO_H_ */