/**
 @file    common.h
 @author  Roberto Carta
 @version 1.0
 @date    02/11/2022
 
 @brief   Header file with common declarations
 
 Common declaration to support the development of embedded applications 
 for the  msp432p401r Launchpad board.
 The output_ref_t type (output pin reference type) can be used to refer to any output pin, 
 connected to an even or odd port, as long as it must be configured as output.
 A LED connected to a MCU pin is an example of an output pin.
 
 The input_pinref_t type (input pin reference type) can be used to refer to any input pin, 
 connected to an even or odd port, as long as it must be configured as input.
 A button connected to a MCU pin is an example of an input pin.
*/
#ifndef COMMON_H
#define COMMON_H

/* SECTION 1: Included header files required to compile this file  */
#include <stdint.h>
#include "ti/devices/msp432p4xx/inc/msp.h"

/* SECTION 2: Public macros                                        */

/* SECTION 3: Public types                                         */

/**
 @brief Datatype used to reference a general-purpose output pin
 @remarks Only one of the odd or even pointers can be used per pin reference 
*/
struct output_ref_s {
   uint8_t mask;    /**< Bitmask of the associated pin. 
                         Result of BIT(pin), pin in the range [0,7]    */
   uint8_t port_is_odd;  /**< Flag (0/1) the determine which pointer to use */
   union {
      DIO_PORT_Odd_Interruptable_Type  *odd;  /**< Use this in case the port is odd: P1, P3, ... */
      DIO_PORT_Even_Interruptable_Type *even; /**< Use this in case the port is even: P2, P4, ... */
   };
};

/**
 @brief Short alias "output_ref_t" for the data type "struct output_ref_s"
*/
typedef struct output_ref_s output_ref_t;


/**
 @brief Datatype used to reference a general-purpose input pin
 An input pin contains the same information as an output pin plus 
    - whether an internal pull-up resistor is required in the configuration (use_pullup field).
	- whether interrupts are used or not (use_interrupt field).
    - the interupt number (int_num field).
 @note Only values INT_PORT1 to INT_PORT6 (declared in driverlib's interrupt.h header file)
 are expected to be used in the int_num field.
 @remarks Only one the the odd and even pointers can be used per pin reference 
*/
struct input_ref_s {
   uint8_t  mask;          /**< Bitmask of the associated pin. 
                                Result of BIT(pin), pin in the range [0,7]  */
   uint8_t  port_is_odd;   /**< Flag (0/1) to know which pointer to use     */
   uint8_t  use_pullup;    /**< Flag (0/1) to know if the internal 
                                pull-up resistor is required.               */
   uint8_t  use_interrupt; /**< Flag (0/1) to know if the input pin is 
                                managed using polling or interrupts         */
   uint16_t int_num;       /**< Interrupt number (INT_PORT1 to INT_PORT6), 
                                used only if use_interrupt == 1.            */
   union {
      DIO_PORT_Odd_Interruptable_Type  *odd;  /**< Use this in case the port 
                                                   has an odd number: 
                                                   P1, P3, P5, ...          */
      DIO_PORT_Even_Interruptable_Type *even; /**< Use this in case the port 
                                                   has an even number: 
                                                   P2, P4, P6, ...          */
   };
};

/**
 @brief Short alias "input_pinref_t" for the data type "struct input_ref_s"
*/
typedef struct input_ref_s input_ref_t;

/* SECTION 4: Public variables :: declarations, extern mandatory   */

/* SECTION 5: Public functions :: declarations, extern optional
   Rule exception (callbacks)  :: declarations, extern recommended */

#endif // COMMON_H
