Password-activated-door
========================

This project allows you to open the door of your home by entering your password.

Components used in this project:
--------------------------------
(1)ATmega 32.

(2)LCD 20*2.

(3)Keypad 4*3.

(4)DC Motor.

(5)Push button switch.

Features of the project:
-------------------------
(1)Automatically opening the door using password.

(2)Automatically closing the door (if push button is pressed).

(3)Ability to change the password.

(4)Secure as if you entered the password 9 times wrong, the door 
   will be closed permanently.

----------------------------------------------------------------

N.B: Before getting into the overview of the code, the password is set initially to "1234" and saved in EEPROM.

Overview of the code:
---------------------

First of all, a welcome message will be appeared on LCD and the program waits for you to enter any key.

Secondly and after pressing any key, a choose list will be appeared on LCD asking you to choose one of two operations:

(1)Enter password.

(2)Change your password.

----------------------------------------------------------------

(1)Entering the password:
-------------------------
If you pressed '1' using the keypad, that means you want to enter the password, the code will ask you to enter the password (which is "1234" if you don't change it).
If the password is right, the DC Motor will rotate in clock wise direction which means that the door is opened.
However, if the password is wrong, the door won't open (and absloutely the DC Motor won't rotate).

Short notes about entering the password:

(1) If you entered the password three times wrong, it will block you from re-entering the password for 60 seconds and then makes you able to try again.

(2) If you entered the password three times wrong again, it will block you for 60 seconds again.

(3) If you entered the password three times wrong again for the third time, the door will be closed permanently and you won't be able to enter the password again.

In short, the code gives you 9 trials to enter the password correctly, and after the 9 trials the door will be closed.

---------------------------------------------------------------

(2)Changing the password:
--------------------------
If you pressed '2' using the keypad, that means you want to change the password, the code will ask you to enter the old password (which is "1234" if you don't change it) and then to enter the new password two times. You must enter the same new passord two times. The new password will be saved in EEPROM.

--------------------------------------------------------------

Notes:
------
The push button switch must be put in somewhere so that if the door is closed, the push button will be pressed. The push button is connected to external intrrupt 2 (which is PB2 in ATmega32). If the push button is pressed, the door will be closed.


---------------------------------------------------------------

About the attached files:
--------------------------
(1)This code is implemented on AVR studio 6 IDE. So, it is highly recommended to download it and begin using my code.

(2)A proteus simulation is also attached.

----------------------------------------------------------------

Future work:
------------
-Battery-indicator to help the user to track the battery status.