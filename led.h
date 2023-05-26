/**
 @file    led.h
 
 @brief   LEDs management for the msp432p401r Launchpad board

 @author  Roberto Carta
 @version 1.0
 @date    18/02/2022
*/

// Do not write above this line (except comments)!
#ifndef LED_H
#define LED_H

/* SECTION 1: Included header files required to compile this file  */


/* SECTION 2: Public macros                                        */

#define LED0        0
#define LED1_RED    1
#define LED1_GREEN  2
#define LED1_BLUE   3
#define LED2_RED    4
#define LED2_GREEN  5
#define LED2_BLUE   6


/* SECTION 3: Public types                                         */


/* SECTION 4: Public variables :: declarations, extern mandatory   */


/* SECTION 5: Public functions :: declarations, extern optional
   Rule exception (callbacks)  :: declarations, extern recommended */

void ledsInit(void); //Initialization function

int ledsGetNum(void); //Retrieve the number of LEDs in the system

int ledOn(int which_led); //Switch a LED on

int ledOff(int which_led); //Switch a LED on

int ledToggle(int which_led); //Toggle a LED

int ledGet(int which_led); //Retrieve the status of a LED



#endif //LED_H
// Do not write below this line!

