/**
 @file    led.c
 @author  Roberto Carta
 @version 1.0
 @date    18/02/2022
 
 @brief   LEDs management for the msp432p401r Launchpad board
*/

// Do not write above this line (except comments)!
/* SECTION 1: Included header files to compile this file           */
#include "common.h"
#include "led.h"


/* SECTION 2: Private macros                                       */

/**
 @brief Macro to automatically calculate the number of entries in the @sa _ledsPinRefs array
*/
#define NUM_LEDS (sizeof(_ledPinRefs)/sizeof(output_ref_t))


/* SECTION 3: Private types                                        */


/* SECTION 4: Public variables  :: definitions, no extern 
   (must match declarations in header file)                        */


/* SECTION 5: Private variables :: definitions, static mandatory 
  (no need to declare, definitions include declarations)           */

/**
 @brief Private array of pin references for leds on the board
 
 @remark This is the only element that should be adapted to
 accommodate a different number of LEDs in the board, 
 or LEDs located at different pins/ports.
*/
static output_ref_t _ledPinRefs [] = {
                                      {. mask = BIT0 , . port_is_odd = 1, .odd = P1}, // LED0 P1 .0
                                      {. mask = BIT0 , . port_is_odd = 0, . even = P2}, // LED1_RED P2 .0
                                      {. mask = BIT1 , . port_is_odd = 0, . even = P2}, // LED1_GREEN P2 .1
                                      {. mask = BIT2 , . port_is_odd = 0, . even = P2}, // LED1_BLUE P2 .2
                                      {. mask = BIT6 , . port_is_odd = 0, . even = P2}, // LED2_RED P2 .6
                                      {. mask = BIT4 , . port_is_odd = 0, . even = P2}, // LED2_GREEN P2 .4
                                      {. mask = BIT6 , . port_is_odd = 1, .odd = P5} // LED2_BLUE P5 .6
};

/* SECTION 6: Private functions :: declarations, static mandatory
   Rule exception (ISRs)        :: declarations, no static         */

static void _ledInitOdd(DIO_PORT_Odd_Interruptable_Type *port, uint8_t mask); //Initialization function for a single LED connected to an odd port (P1, P3, etc.)

static void _ledInitEven(DIO_PORT_Even_Interruptable_Type *port, uint8_t mask); //Initialization function for a single LED connected to an even port (P2, P4, etc.)

static void _ledInit(output_ref_t *pin); //Initialization function for a single LED

/* SECTION 7: Private functions :: definitions, static mandatory
   Rule exception (ISRs)        :: definitions, no static 
   Public functions             :: definitions, no extern
   Function definitions (private & public) written in any order    */
void ledsInit(void)
{
    uint16_t j;
    for(j = 0; j < NUM_LEDS; j++)
    {
        _ledInit( &(_ledPinRefs[j]) );
        ledOff(j);
    }
}

int ledsGetNum(void)
{
    return NUM_LEDS;
}

int ledOn(int which_led)
{
    if(which_led < 0 || which_led > NUM_LEDS-1)
        return -1;


    if(_ledPinRefs[which_led].port_is_odd)
        _ledPinRefs[which_led].odd->OUT =  _ledPinRefs[which_led].odd->OUT | _ledPinRefs[which_led].mask;

    else
        _ledPinRefs[which_led].even->OUT =  _ledPinRefs[which_led].even->OUT | _ledPinRefs[which_led].mask;  //in this case it won't be used but in the future it could be

    return 1;
}


int ledOff(int which_led) //Switch a LED on
{
    if(which_led < 0 || which_led > NUM_LEDS-1)
            return -1;

    if(_ledPinRefs[which_led].port_is_odd)
            _ledPinRefs[which_led].odd->OUT =  _ledPinRefs[which_led].odd->OUT & ~(_ledPinRefs[which_led].mask);

        else
            _ledPinRefs[which_led].even->OUT =  _ledPinRefs[which_led].even->OUT & ~(_ledPinRefs[which_led].mask);  //in this case it won't be used but in the future it could be

    return 1;
}

int ledToggle(int which_led) //Toggle a LED
{
    if(which_led < 0 || which_led > NUM_LEDS-1)
        return -1;

    if(ledGet(which_led))
        ledOff(which_led);

    else
        ledOn(which_led);

    return 1;
}

int ledGet(int which_led) //Retrieve the status of a LED
{
    if(which_led < 0 || which_led > NUM_LEDS-1)
        return -1;

    if(_ledPinRefs[which_led].port_is_odd)
        return (_ledPinRefs[which_led].odd->OUT & _ledPinRefs[which_led].mask) == _ledPinRefs[which_led].mask;

    else
        return (_ledPinRefs[which_led].even->OUT & _ledPinRefs[which_led].mask) == _ledPinRefs[which_led].mask;  //in this case it won't be used but in the future it could be

}


static void _ledInitOdd(DIO_PORT_Odd_Interruptable_Type *port, uint8_t mask) //Initialization function for a single LED connected to an odd port (P1, P3, etc.)
{
    port->SEL0 = port->SEL0 & ~mask;
    port->SEL1 = port->SEL1 & ~mask;
    port->DIR = port->DIR | mask;
}

static void _ledInitEven(DIO_PORT_Even_Interruptable_Type *port, uint8_t mask) //Initialization function for a single LED connected to an even port (P2, P4, etc.)
{
    port->SEL0 = port->SEL0 & ~mask;
    port->SEL1 = port->SEL1 & ~mask;
    port->DIR = port->DIR | mask;
}

static void _ledInit(output_ref_t *pin) //Initialization function for a single LED
{
    if(pin->port_is_odd)
        _ledInitOdd(pin->odd , pin->mask);

    else
        _ledInitEven(pin->even , pin->mask);
}
