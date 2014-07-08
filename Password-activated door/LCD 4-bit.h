/*
 * LCD4-bit.h
 *
 * Created: 11/16/2013 11:25:28 AM
 * Author: Armia Wagdy
 */ 


#ifndef LCD4bit_H_
#define LCD4bit_H_

#include <avr/io.h>
#define F_CPU 1000000UL

#define	LCD_PRT	PORTA			//LCD Data PORT
#define	LCD_DDR	DDRA			//LCD Data DDR
#define	LCD_PIN	PINA			//LCD Data PIN
#define	LCD_RS	0				//LCD RS
#define	LCD_RW  1				//LCD RW
#define	LCD_EN	2				//LCD EN

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20                                           // 4-bit interface, single line, 5x7 dots
#define LCD_FUNCTION_4BIT_2LINES 0x28                                           // 4-bit interface, dual line,   5x7 dots
#define LCD_FUNCTION_8BIT_1LINE  0x30                                           // 8-bit interface, single line, 5x7 dots
#define LCD_FUNCTION_8BIT_2LINES 0x38                                           // 8-bit interface, dual line,   5x7 dots

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08                                           // display off
#define LCD_DISP_ON              0x0C                                           // display on, cursor off
#define LCD_DISP_ON_BLINK        0x0D                                           // display on, cursor off, blink char
#define LCD_DISP_ON_CURSOR       0x0E                                           // display on, cursor on
#define LCD_DISP_ON_CURSOR_BLINK 0x0F                                           // display on, cursor on, blink char

/* Some useful commands (Execution time of this commands = 2ms) */
#define CLEAR_DISPLAY            0x01
#define RETURN_HOME              0x02

/* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04                                           // display shift off, dec cursor move dir
#define LCD_ENTRY_DEC_SHIFT      0x05                                           // display shift on,  dec cursor move dir
#define LCD_ENTRY_INC_           0x06                                           // display shift off, inc cursor move dir
#define LCD_ENTRY_INC_SHIFT      0x07                                           // display shift on,  inc cursor move dir

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10                                           // move cursor left  (decrement)
#define LCD_MOVE_CURSOR_RIGHT    0x14                                           // move cursor right (increment)
#define LCD_MOVE_DISP_LEFT       0x18                                           // shift display left
#define LCD_MOVE_DISP_RIGHT      0x1C                                           // shift display right

//******************************************************************************

void lcdCommand(unsigned char cmnd);
void lcdData (unsigned char data);
void lcd_init();
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_print(char *str);
void lcd_clear(void);
void lcd_generate(unsigned char location, unsigned char *ptr);
void lcd_cursor_on(void);
void lcd_cursor_off(void);

#endif /* LCD4bit_H_ */