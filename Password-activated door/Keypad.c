/*
 * Keypad.c
 *
 * Created: 11/16/2013 11:25:28 AM
 * Author: Armia Wagdy
 */ 

//P0 - P3 Connected to columns
//P4 - P7 Connected to rows

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "Keypad.h"
#include "KeypadConfig.h"

unsigned char keypad[4][4] = { '1','2','3','/',
							   '4','5','6','=',
							   '7','8','9','-',
                               '*','0','#','+'};


extern unsigned char get_key (void)
{
  unsigned char colloc, rowloc;
  
  KEY_DDR = 0xF0;                                                               //Columns bits are output and Rows' bits are input
  KEY_PRT = 0xFF;  
  KEY_PIN = 0x0F;
  
      do
          {
              KEY_PRT &= 0x0F;                                                  //ground all rows at once
              colloc = (KEY_PIN & 0x0F);                                        //read the columns
          } while(colloc != 0x0F);                                              //check untill all keys are released
      do
        { 
            do
              {
                  _delay_ms(20);                                        //Delay to allow the user to enter the required Key
                  colloc = (KEY_PIN & 0x0F);                                    //read the columns
              } while(colloc != 0x0F);                                          //wait for key press
          
            _delay_ms(20);                                              //Delay for debounce
            colloc = (KEY_PIN & 0x0F);                                          //read the columns
        } while(colloc != 0x0F);                                                //wait for key press
      
      while(1)
      {
        KEY_PRT = 0xEF;                                                         //ground row 0
        colloc = (KEY_PIN & 0x0F);                                              //read the columns
        if (colloc != 0x0F)                                                     //column detected
        {
          rowloc = 0;                                                           //save row location 
          break;                                                                //exit while loop                                
        }
        KEY_PRT = 0xDF;                                                         //ground row 1
        colloc = (KEY_PIN & 0x0F);                                              //read the columns
        if (colloc != 0x0F)                                                     //column detected
        {
          rowloc = 1;                                                           //save row location 
          break;                                                                //exit while loop                                
        }
        KEY_PRT = 0xBF;                                                         //ground row 2
        colloc = (KEY_PIN & 0x0F);                                              //read the columns
        if (colloc != 0x0F)                                                     //column detected
        {
          rowloc = 2;                                                           //save row location 
          break;                                                                //exit while loop                                
        }
        KEY_PRT = 0x7F;                                                         //ground row 3
        colloc = (KEY_PIN & 0x0F);                                              //read the columns
        if (colloc != 0x0F)                                                     //column detected
        {
          rowloc = 3;                                                           //save row location 
          break;                                                                //exit while loop                                
        }
      }
      
      
     //check column and save result in key
      if(colloc == 0x0E)
        return (keypad[rowloc][0]);
      else if(colloc == 0x0D)
        return (keypad[rowloc][1]);
      else if(colloc == 0x0B)
        return (keypad[rowloc][2]);
      else 
        return (keypad[rowloc][3]);  
}  
