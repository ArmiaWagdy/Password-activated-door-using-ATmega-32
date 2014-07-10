/*
 * Password_activated_door.c
 *
 * Created: 7/6/2014 11:25:15 AM
 * Author: Armia Wagdy
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define  F_CPU 1000000UL
#include <util/delay.h>
#include "LCD 4-bit.h"
#include "Keypad.h"
#include "KeypadConfig.h"
#include "ADC.h"
#include "EEPROM.h"
#include "DIO.h"
#include "external_interrupts.h"

#define  PASSWORD_LENGTH	 10
#define  DEBUG				 0
#define  SETUP				 0
#define  MOTOR_IN1			 0
#define  MOTOR_IN2			 1
#define  SW					 2
#define  MOTOR_EN			 3
#define  BUZZER				 4
#define  NUM_OF_TRIALS_ADDR  25
static char str[100] = {0};
	
	
//************************************** Declaration of functions *********************************************/
void init (void);
void welcome_message (void);
void choose_list (void);
void sign_up (void);
void log_in (void);
void change_password(void);
unsigned char get_password(unsigned char* pass);
unsigned char check_password(unsigned char len, unsigned char* password);
unsigned char get_puk_code(void);
unsigned char compare(unsigned char*, unsigned char*, unsigned char, unsigned char);
void open_the_door (void);
void close_the_door (void);
void try_again (void);
unsigned char is_door_is_permanently_closed(void);

/*************************************************************************************************************/

int main(void)
{			
	unsigned char choice = 0;			
	init();
	
	while(1)
	{
		//Check firstly if the door is permanently locked or not (if the user enters the password 9 times wrong,
		//the door will be locked permanently.
		if(eeprom_read(26) == 0xFF)
		{			
			unsigned char chk = 0;
			do
			{
				chk = get_puk_code();
				
			} while (!chk);
			
		
			lcd_clear();
			lcd_print("Successful operation");
			eeprom_write(26,0);
			eeprom_write(NUM_OF_TRIALS_ADDR,0);
			_delay_ms(1000);
			sign_up();
		}		
		//Print Hello message on screen
		welcome_message();
		
		//Press any key to go to choose list
		get_key();
		
		do 
		{
			choose_list();
			choice = get_key();
		} while (choice != '1' && choice != '2');
		
		if(choice == '1')
		{
			log_in();
		}
		else if(choice == '2')
		{
			change_password();
			//sign_up();
		}			
	}		
		
}

/*************************************************************************************************************/

//This function initiates the LCD, ADC Modules and ports needed

void init (void)
{
	unsigned char i = 0;							//Just a counter
	lcd_init();										//Initialize LCD
	adc_init(128,AVCC,RIGHT);						//Initialize ADC
	DIO_pinmode('B', MOTOR_IN1, OUTPUT);			//PB0 is an output pin
	DIO_pinmode('B', MOTOR_IN2, OUTPUT);			//PB1 is an output pin
	DIO_pinmode('B', MOTOR_EN, OUTPUT);				//PB3 is an output pin
	DIO_pinmode('B', BUZZER, OUTPUT);				//PB4 is an output pin
	DIO_pinmode('B', SW, INPUT_PULLUP);				//PD0 is an input pin (pull-up)
	set_externalInterrupt(INT2, FALLING_EDGE);		//Enable INT0, FALLING_EDGE
	sei();
	eeprom_write(NUM_OF_TRIALS_ADDR,0);
	
#if SETUP
	eeprom_write(1,4);
	for(i = 1; i < 5; i++)
		eeprom_write(i+1, i+48);
	for(i = 0; i < 10; i++)
		eeprom_write(i+27, i+48);
#endif
}

/*************************************************************************************************************/

//This function prints a welcome message to the user

void welcome_message (void)
{
	lcd_clear();
	lcd_print(" Password-lock door");
	lcd_gotoxy(1,2);
	lcd_print("      Welcome!");
}

/*************************************************************************************************************/

//This function lists the possible choices for user

void choose_list (void)
{
	lcd_clear();
	lcd_print("(1)Log-in");
	lcd_gotoxy(1,2);
	lcd_print("(2)Change Password");
}

/*************************************************************************************************************/

//This function asks the user to enter his desired password two times. If the passwords are 
//matched, the password will be saved. Else, It won't and the user should try again.

void sign_up (void)
{
	unsigned char desired_password_1[11] = {0};		//The first entered password is saved here
	unsigned char desired_password_2[11] = {0};		//The second entered password is saved here
	unsigned char length_1 = 0;						//Length of first entered password
	unsigned char length_2 = 0;						//Length of second entered password

	lcd_clear();
	lcd_gotoxy(1,1);
	lcd_print("Your new Password: ");
	lcd_gotoxy(1,2);
	length_1 = get_password(desired_password_1);
	
	lcd_clear();
	lcd_gotoxy(1,1);
	lcd_print("Re-enter Password: ");
	lcd_gotoxy(1,2);
	length_2 = get_password(desired_password_2);
	
	
	if(compare(desired_password_1,desired_password_2,length_1,length_2))
	{
		unsigned char i = 0;		//Just a counter variable
		cli();
		
		//Save the length of the password in EEPROM (Location = 0x0001)
		eeprom_write(0x0001,length_1);
		
		for(i = 0; i < length_1; i++)
		{
			//Save the password in EEPROM (In location from 12 to 22)
			eeprom_write(i+2,desired_password_1[i]);
		}
		sei();
		
		//Successful operation
		lcd_clear();
		lcd_print("  Your password is");
		lcd_gotoxy(1,2);
		lcd_print("      is saved");
		_delay_ms(1000);
	}
	
	else
	{
		//Failed operation, try again
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Failed operation");
		lcd_gotoxy(1,2);
		lcd_print("Plz, Try again");
		_delay_ms(1000);
		sign_up();
	}
}

/*************************************************************************************************************/

//This function compares password_1 and password_2 and returns '1' if they are matched and '0' 
//if matching didn't occur.

unsigned char compare(unsigned char* pass1, unsigned char* pass2, unsigned char len1, unsigned char len2)
{
	unsigned char i = 0;		//Just a counter variable

	if(len1 != len2)
	{
		
#if DEBUG
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("! = ");
		_delay_ms(1000);
#endif
		return 0;
	}
	
	else
	{
		for(i = 0; i < len1; i++)
		{
			if(pass1[i] != pass2[i])
			{
#if DEBUG
				lcd_clear();
				lcd_gotoxy(1,1);
				lcd_print("pass1 = ");
				lcd_print(itoa((int)pass1[i],(char*)str,10));
				lcd_gotoxy(1,2);
				lcd_print("pass2 = ");
				lcd_print(itoa((int)pass2[i],(char*)str,10));
				_delay_ms(500);
#endif
				return 0;
			}
			else
			{
#if DEBUG
				lcd_clear();
				lcd_gotoxy(1,1);
				lcd_print("pass1 = ");
				lcd_print(itoa((int)pass1[i],str,10));
				lcd_gotoxy(1,2);
				lcd_print("pass2 = ");
				lcd_print(itoa((int)pass2[i],str,10));
				_delay_ms(2000);
#endif				
				continue;			//No need for it, just to make the code readable
			}
		}
		return 1;
	}
}

/*************************************************************************************************************/

//This function tells the user to enter the saved password and then checks if it is right or not
//using check_password function. And then make a decision upon the results

void log_in (void)
{
	unsigned char entered_password[11] = {0};				//The entered password is saved here
	unsigned char length = 0;								//Length of entered password
	
	lcd_clear();
	lcd_print("Enter your password: ");
	lcd_gotoxy(1,2);
	length = get_password(entered_password);
	
	if(check_password(length, entered_password))
	{
		open_the_door();
	}
	else
	{
		try_again();
	}
}

/*************************************************************************************************************/

//This function changes the password of user. It firstly asks to enter his old password and if it is right, It
//will ask you to enter your new password two times. 

void change_password(void)
{
	unsigned char old_password[10] = {0};		//Old password is stored here
	unsigned char length = 0;					//length of old password is stored here
	
	lcd_clear();
	lcd_print("Enter old password:");
	lcd_gotoxy(1,2);
	length = get_password(old_password);
	if (check_password(length, old_password))
	{
		sign_up();
	}
	else
	{
		
		change_password();
	}
}


/*************************************************************************************************************/
//This function takes the password from the user

unsigned char get_password(unsigned char* pass)
{
	int cnt = 0;
	unsigned char key = 0;
	
	lcd_cursor_on();
	
	for(cnt = 0; cnt <= 10 ; )
	{
		key = get_key();
		
		if(key == '#')
		{
			break;
		}
		
		else if(key == '*' && cnt > 0)
		{
			lcd_move_cursor_left();
			lcdData(' ');
			lcd_move_cursor_left();
			cnt --;	
		}
		else if(key == '*' && cnt <= 0)
		{
			cnt = 0;
		}
		else if(cnt != 10)
		{
			pass[cnt++] = key;
			lcdData('*');
		}
	}
	
	//do
	//{
		//pass[cnt] = get_key();
		//
		//if(pass[cnt] == '*')
		//{
			//cnt++;
		//}
		//else
		//{
			////lcdData(pass[cnt++]);
			//lcdData('*');
			//cnt++;
		//}		
		//if(cnt == PASSWORD_LENGTH)
		//{
			//while(get_key() != '*');
			//pass[cnt++] = '*';
		//}
	//} while (pass[cnt-1] != '*');
	
	lcd_cursor_off();
	
	return cnt;
}

/*************************************************************************************************************/

//This function compare the entered password with the one saved in the EEPROM. If matching occurs,
//it would return '1'. Else, It would return '0'

unsigned char check_password(unsigned char len, unsigned char* password)
{	
	unsigned char i = 0;				//Just a counter variable
	
	if(eeprom_read(0x0001) != len)		//Length is stored in EEPROM location 0x0001
	{
		return 0;
	}
	else
	{
		for(i = 0; i < len; i++)
		{
			if(password[i] != eeprom_read(i+2))		//Password is stored in EEPROM from location 2 to 12
			{
				lcd_clear();
				lcd_print("break");
				_delay_ms(1000);
				return 0;
			}
		}
		return 1;
	}
}

/*************************************************************************************************************/

//This function rotates the motor in counter_clock wise direction which means that the door is closed

void open_the_door (void)
{
	DIO_write('B', MOTOR_EN, HIGH);		//EN = 1
	DIO_write('B', MOTOR_IN1, HIGH);	//IN1 = 1
	DIO_write('B', MOTOR_IN2, LOW);		//IN2 = 0
	lcd_clear();
	lcd_print(" The door is opened");
	_delay_ms(2000);
	DIO_write('B', MOTOR_EN, LOW);		//EN = 0
	eeprom_write(NUM_OF_TRIALS_ADDR,0);
}

/*************************************************************************************************************/

//This function rotates the motor in counter_clock wise direction which means that the door is closed

void close_the_door (void)
{	
	
	DIO_write('B', MOTOR_EN, HIGH);		//EN = 1
	DIO_write('B', MOTOR_IN1, LOW);	//IN1 = 0
	DIO_write('B', MOTOR_IN2, HIGH);		//IN2 = 1
	DIO_write('B', BUZZER, HIGH);		//Make some noise :D
	PORTB |= (1 << BUZZER);
	_delay_ms(2000);
	DIO_write('B', BUZZER, LOW);
	DIO_write('B', MOTOR_EN, LOW);		//EN = 0
	
}

/*************************************************************************************************************/

//If the voltage on PB2 changes from HIGH to LOW (falling_edge) the door will be closed.
//In reality the interrupt can come from any source .... can be a sensor connected to the door or just 
//a simple switch when pressed, the door will be closed.

ISR(INT2_vect)
{
	cli();
	close_the_door();
	sei();
}

/*************************************************************************************************************/

void try_again (void)
{
	static unsigned char number_of_trials = 0;
	number_of_trials = eeprom_read(NUM_OF_TRIALS_ADDR);
	eeprom_write(NUM_OF_TRIALS_ADDR,++number_of_trials);			//This variable is stored in EEPROM location number 25
	
	if(eeprom_read(NUM_OF_TRIALS_ADDR) == 3 || eeprom_read(NUM_OF_TRIALS_ADDR) == 6)
	{
		unsigned char i = 0;
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Wrong password");
		lcd_gotoxy(1,2);
		lcd_print("Try again in    Sec");
		for(i = 59; i > 0; i--)
		{	
			lcd_gotoxy(14,2);
			itoa((int)i, str, 10);
			lcd_print(str);
			_delay_ms(100);
		}
		log_in();
	}
	
	else if (eeprom_read(NUM_OF_TRIALS_ADDR) >= 9)
	{
		eeprom_write(26,0xFF);
		unsigned char chk = 0;
		do
		{
			chk = get_puk_code();
			
		} while (!chk);
		
		lcd_clear();
		lcd_print("Successful operation");
		eeprom_write(26,0);
		eeprom_write(NUM_OF_TRIALS_ADDR,0);
		_delay_ms(1000);
		sign_up();
	}
	
	else
	{
		lcd_clear();
		lcd_gotoxy(1,1);
		lcd_print("Wrong password");
		lcd_gotoxy(1,2);
		lcd_print("Plz, try again");
		_delay_ms(1000);
		log_in();
	}
}

/*************************************************************************************************************/

//This function checks if the user enters the puk code correctly or not

unsigned char get_puk_code(void)
{
	unsigned char i = 0;				//Just a counter variable
	unsigned char puk_code[10] = {0};
	unsigned char length = 0;
	
	lcd_clear();
	lcd_print(" The door is closed ");
	lcd_gotoxy(1,2);
	lcd_print("    permanently");
	_delay_ms(1000);
	
	lcd_clear();
	lcd_print("Enter PUK code: ");
	lcd_gotoxy(1,2);
	length = get_password(puk_code);
	for(i = 0; i < 10; i++)
		puk_code[i] -= 48;
		
	if(length != 10)		//PUK code is 10 numbers + the ending '*' which acts as an enter.
	{
		lcd_clear();
		lcd_print(itoa((int)length,str,10));
		_delay_ms(1000);
		return 0;
	}
	else
	{
		for(i = 0; i < 10; i++)
		{
			if(puk_code[i] != eeprom_read(i+27))		//Password is stored in EEPROM from location 27 to 37
			{
						lcd_clear();
						lcd_print(itoa(i,str,10));
						_delay_ms(1000);
				return 0;
			}
		}
		return 1;
	}	
}

/*************************************************************************************************************/