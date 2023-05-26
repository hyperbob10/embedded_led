/**
 @file    button.h
 @author  Roberto Carta
 @version 1.0
 @date    14/03/2022
 
 @brief   Button designator

*/

// Do not write above this line (except comments)!
#ifndef BUTTON_H
#define BUTTON_H

/* SECTION 1: Included header files required to compile this file  */


/* SECTION 2: Public macros                                        */
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3

/* SECTION 3: Public types                                         */


/* SECTION 4: Public variables :: declarations, extern mandatory   */


/* SECTION 5: Public functions :: declarations, extern optional
   Rule exception (callbacks)  :: declarations, extern recommended */
void buttonsInit(void);              //Initialization function

int buttonState(int which_button);   //Get the current button state

int buttonPressed(int which_button); //Determine if the button has been pressed since the last time this function was called

extern void buttonCallback(int which_button);

#endif // BUTTON_H
// Do not write below this line!

