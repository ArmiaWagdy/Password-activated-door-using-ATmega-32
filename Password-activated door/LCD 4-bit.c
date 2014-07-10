/*
 * LCD 4-bit.c
 *
 * Created: 11/16/2013 11:24:46 AM
 *  Author: A R M T
 */ 
#include "LCD 4-bit.h"
#include <util/delay.h>

//*******************************************************************************
void delay_us(int d)
{
	while(d--) 
	{
		_delay_us(1);
	}
}
//*******************************************************************************
void delay_ms(int d)
{
	while(d--)
	{
		_delay_ms(1);
	}
}
//******************************************************************************
void lcd_reset(void)
{
	LCD_DDR = 0xFF;					                        //LCD port is output
	LCD_PRT &= ~(1<<LCD_EN);			                    //LCD_EN = 0
	delay_us(20000);				                        //wait for stable power (more than 15 ms)
	lcdCommand(0x03);				                        //$30 for reset
	delay_us(10000);				                        //wait for more than (4.1 ms)
    lcdCommand(0x03);										//$30 for reset
	delay_us(1000);											//wait (for more than (100 us)
    lcdCommand(0x03);										//$30 for reset
	delay_us(1000);                                         //wait (for more than 100 us)
    lcdCommand(0x02);										//$20 for reset
	delay_us(1000);                                         //wait (for more than 100 us)
}
//******************************************************************************
void lcd_init(void)
{       
    //lcd_reset();
	LCD_DDR = 0xFF;
	delay_us(2000);
    /* Function set */
    lcdCommand(0x33);							   //$28 for 4-bit mode
	delay_us(100);                                                     //wait
        
    /* Display ON/OFF Control */
	lcdCommand(0x32);                               //display on, cursor on, blinking
	delay_us(100);														//wait
        
   
	lcdCommand(LCD_FUNCTION_4BIT_2LINES);
	delay_us(100);					                        //clear LCD
	
	lcdCommand(LCD_DISP_ON);
	delay_us(100);
	
	lcdCommand(CLEAR_DISPLAY);
	delay_us(2000);														//wait
        
    /* Entry mode set */
	lcdCommand(LCD_ENTRY_INC_);									//shift && cursor right
	delay_us(100);                                                     //wait
}
//******************************************************************************
void lcdCommand(unsigned char cmnd)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (cmnd & 0xF0);                             //taking high nibble of cmnd
	LCD_PRT &= ~(1<<LCD_RS);	       		                        //RS = 0 for command (IR Register)
	LCD_PRT &= ~(1<<LCD_RW);			                        //RW = 0 for Write
 	LCD_PRT |= (1<<LCD_EN);			                                //EN = 1 for H-to-L			                                //wait to make EN wider (min 450 ns)
	delay_us(1);
	LCD_PRT &= ~(1<<LCD_EN);		                                //EN = 0 for H-to-L	
        
	delay_us(20);                                                     //Wait some time
        
	LCD_PRT = (LCD_PRT & 0x0F) | (cmnd << 4);                               //taking low nibble of cmnd
	LCD_PRT |= (1<<LCD_EN);			                                //EN = 1 for H-to-L
	delay_us(1);			                                //wait to make enable wider (min 450 ns)
	LCD_PRT &= ~(1<<LCD_EN);		                                //EN = 0 for H-to-L
}
//******************************************************************************
void lcdData (unsigned char data)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (data & 0xF0);	                        //taking high nibble of data
	LCD_PRT |= (1<<LCD_RS);												//RS = 1 for data (DR Register)
	LCD_PRT &= ~(1<<LCD_RW);											//RW = 0 for Write
	LCD_PRT |= (1<<LCD_EN);												//EN = 1 for H-to-L
																		 
	delay_us(1);														//wait to make enable wider (min 450 ns)
	
	LCD_PRT &= ~(1<<LCD_EN);											//EN = 0 for H-to-L    
	LCD_PRT = (LCD_PRT & 0x0F) | (data << 4);                           //taking low nibble of data
	LCD_PRT |= (1<<LCD_EN);						                        //EN = 1 for H-to-L
	delay_us(1);							                            //wait to make enable wider
	LCD_PRT &= ~(1<<LCD_EN);					                        //EN = 0 for H-to-L
}
//******************************************************************************
void lcd_gotoxy(unsigned char x, unsigned char y)
{
	unsigned char firstcharAdr[] = {0x80, 0xC0, 0x94, 0xD4};
	lcdCommand(firstcharAdr[y-1] + x - 1);
	delay_us(100);
}
//*******************************************************************************
void lcd_cursor_on(void)
{
	lcdCommand(LCD_DISP_ON_CURSOR_BLINK);
	delay_us(100);
}
//*******************************************************************************
void lcd_cursor_off(void)
{
	lcdCommand(LCD_DISP_ON);
	delay_us(100);
}
//*******************************************************************************
void lcd_move_cursor_left(void)
{
	lcdCommand(LCD_MOVE_CURSOR_LEFT);
	delay_us(100);
}

//*******************************************************************************
void lcd_move_cursor_right(void)
{
	lcdCommand(LCD_MOVE_CURSOR_RIGHT);
	delay_us(100);
}

//*******************************************************************************
void lcd_print(char *str)
{
	unsigned char i = 0;
	
	while(str[i] != 0)
	{
		lcdData(str[i]);
		i++;
	}
}
//*******************************************************************************
void lcd_clear(void)
{
	lcdCommand(CLEAR_DISPLAY);
	delay_us(2000);
	lcd_gotoxy(1,1);
}
//*******************************************************************************
//Input:
// location: location where you want to store
// 0,1,2,....7
// ptr: Pointer to pattern data
//
//Usage:
// pattern[8]={0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
// LCD_build(1,pattern);
//
//LCD Ports are same as discussed in previous sections
 
void lcd_generate(unsigned char location, unsigned char *ptr)
{
	unsigned char i;
	if(location<8)
	{
		lcdCommand(0x40+(location*8));
		for(i=0;i<8;i++)
		{
			lcdData(ptr[i]);	
		}	
	}
}

//*******************************************************************************