/**
 @file    lab4.c
 @author  Roberto Carta
 @version 1.0
 @date    Spring 2021
 
 @brief   Simple code to test buttons managed using polling
*/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "led.h"
#include "button.h"

/** @todo
 Add the definition of button module callback function (buttonCallback) in order
 to incorporate the application reaction to interrupt-driven buttons BUTTON1 and BUTTON2
 */

int main(void) {
    volatile uint32_t i;
    int n, l, res;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

   	/* Initialize the led and button modules */
    ledsInit();
    buttonsInit();
	
	/* Enable interrupts in the application  */
	Interrupt_enableMaster();
   	
	/* Superloop: react to polling-based buttons */
    while (1)
    {
		res = buttonState(BUTTON0);
		if (res < 0) {
			while(1);
		}
		if (res == 1) {
			ledOn(LED0);
		} else {
			ledOff(LED0);
		}

		res = buttonPressed(BUTTON3);
		if (res < 0) {
			while(1);
		}
		if (res == 1) {
			do {
				ledToggle(LED1_RED);
				ledToggle(LED1_GREEN);
				ledToggle(LED1_BLUE);
				for (i = 20000; i > 0; i--);
				res = buttonPressed(BUTTON3);
				if (res < 0) {
					while(1);
				}
			} while (res == 0);
		}
    }
}

void buttonCallback(int which_button)
{
    if(which_button == 1)
    {
        ledToggle(LED2_RED);
    }

    else if(which_button == 2)
    {
        ledToggle(LED2_GREEN);
        ledToggle(LED2_BLUE);
    }
}
