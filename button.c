/**
 @file    button.h
 @author  Roberto Carta
 @version 1.0
 @date    14/03/2022
 
 @brief   Button designator

*/

// Do not write above this line (except comments)!
/* SECTION 1: Included header files to compile this file           */
#include "button.h"
#include "common.h"
#include "ti/devices/msp432p4xx/driverlib/driverlib.h"


/* SECTION 2: Private macros

/**
@brief Number of entries in the @sa _pinrefs array
@note This value is automatically calculated , do not edit
*/
#define NUM_BUTTONS (sizeof(_pinrefs) / sizeof(input_ref_t))


/**
@brief Private array of pin references for buttons on the board
*/
static input_ref_t _pinrefs [] = {
     { .mask = BIT1 , .port_is_odd = 1, .odd = P1 , // BUTTON0 P1 .1
       .use_pullup = 1,                             // Internal pull -up
       .int_num = INT_PORT1 , .use_interrupt = 0 // Polling
     },
     { .mask = BIT4 , .port_is_odd = 1, .odd = P1 , // BUTTON1 P1 .4
       .use_pullup = 1,                             // Internal pull -up
       .int_num = INT_PORT1 , .use_interrupt = 1    // Interrupts
     },
     { .mask = BIT1 , .port_is_odd = 1, .odd = P5 , // BUTTON2 P5 .1
       .use_pullup = 0,                             // No internal pull -up
       .int_num = INT_PORT5 , .use_interrupt = 1    // Interrupts
     },
     { .mask = BIT5 , .port_is_odd = 1, .odd = P3 , // BUTTON3 P3 .5
       .use_pullup = 0,                             // No internal pull -up
       .int_num = INT_PORT3 , .use_interrupt = 0    // Polling
     }
};
/* SECTION 3: Private types                                        */


/* SECTION 4: Public variables  :: definitions, no extern 
   (must match declarations in header file)                        */


/* SECTION 5: Private variables :: definitions, static mandatory 
  (no need to declare, definitions include declarations)           */


/* SECTION 6: Private functions :: declarations, static mandatory
   Rule exception (ISRs)        :: declarations, no static         */
static void _buttonInit(input_ref_t *ref); //Initialization function for a single button


static void _buttonProcessFlags(uint16_t int_num , uint8_t flag);

static int _buttonInverseLookup(uint16_t int_num , uint8_t mask);
/* SECTION 7: Private functions :: definitions, static mandatory
   Rule exception (ISRs)        :: definitions, no static 
   Public functions             :: definitions, no extern
   Function definitions (private & public) written in any order    */
void buttonCallback(int which_button) __attribute__((weak));
void buttonCallback(int which_button)
{
    /*empty function*/
}

void PORT1_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P1->IFG & P1->IE;
    P1->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT1 , filtered_buttons);
}
void PORT2_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P2->IFG & P2->IE;
    P2->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT2 , filtered_buttons);
}
void PORT3_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P3->IFG & P3->IE;
    P3->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT3 , filtered_buttons);
}
void PORT4_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P4->IFG & P4->IE;
    P4->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT4 , filtered_buttons);
}
void PORT5_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P5->IFG & P5->IE;
    P5->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT5 , filtered_buttons);
}
void PORT6_IRQHandler(void)
{
    uint8_t filtered_buttons;
    filtered_buttons = P6->IFG & P6->IE;
    P6->IFG &= ~filtered_buttons;
    _buttonProcessFlags(INT_PORT6 , filtered_buttons);
}

static void _buttonProcessFlags(uint16_t int_num , uint8_t flag)
{
    int i,mask,button;

    i = 0; mask = BIT0;

    while(i < 8)
    {
        if((flag & mask) != 0)
        {
            button = _buttonInverseLookup(int_num , mask);
            if(button >= 0)
            {
                buttonCallback(button);
            }

        }

        i++;
        mask = mask << 1;
    }
}


static int _buttonInverseLookup(uint16_t int_num , uint8_t mask)
{
    int i;
    i = 0;

    while(i < NUM_BUTTONS)
    {
        if((_pinrefs[i].mask == mask) && _pinrefs[i].int_num == int_num)
            return i;
        else
            i++;


    }
    return -1;
}

static void _buttonInit(input_ref_t *ref)
{
    if(ref->use_interrupt)
    {
        Interrupt_enableInterrupt(ref->int_num);
    }

    if(ref->port_is_odd)
    {
        DIO_PORT_Odd_Interruptable_Type *odd_port = ref->odd;
        odd_port->DIR = odd_port->DIR & ~(ref->mask);
        odd_port->SEL0 = odd_port->SEL0 & ~(ref->mask);
        odd_port->SEL1 = odd_port->SEL1 & ~(ref->mask);

        if(ref->use_pullup)
        {
            odd_port->REN = odd_port->REN | (ref->mask);
            odd_port->OUT = odd_port->OUT | (ref->mask);
        }

        else
        {
            odd_port->REN = odd_port->REN & ~(ref->mask);
        }

        odd_port->IES = odd_port->IES | ref->mask;

        if(ref->use_interrupt)
        {
            odd_port->IE = odd_port->IE | ref->mask;
        }

        else
        {
            odd_port->IES = odd_port->IES & ~(ref->mask);
        }
    }

    else
    {
        DIO_PORT_Even_Interruptable_Type *even_port = ref->even;
        even_port->DIR = even_port->DIR & ~(ref->mask);
        even_port->SEL0 = even_port->SEL0 & ~(ref->mask);
        even_port->SEL1 = even_port->SEL1 & ~(ref->mask);

        if(ref->use_pullup)
        {
            even_port->REN = even_port->REN | (ref->mask);
            even_port->OUT = even_port->OUT | (ref->mask);
        }

        else
        {
            even_port->REN = even_port->REN & ~(ref->mask);
        }

        even_port->IES = even_port->IES | ref->mask;

        if(ref->use_interrupt)
        {
            even_port->IE = even_port->IE | ref->mask;
        }

        else
        {
            even_port->IES = even_port->IES & ~(ref->mask);
        }
    }

}


void buttonsInit(void)
{
    int i;
    for (i=0; i < NUM_BUTTONS ; i++)
    {
        _buttonInit (& _pinrefs [i]);
    }
}
int buttonState(int which_button)
{
    int res, val;

    if(which_button >= NUM_BUTTONS)
        res = -1;

    else
    {
        uint8_t mask = _pinrefs[which_button].mask;

        if(_pinrefs[which_button].port_is_odd)
        {
            val = _pinrefs[which_button].odd->IN & _pinrefs[which_button].mask;
        }

        else
        {
            val = _pinrefs[which_button].even->IN & _pinrefs[which_button].mask;
        }

        if(val == 0)
            res = 1;
        else
            res = 0;
    }

    return res;
}

int buttonPressed(int which_button)
{
    int res, val;

    if(which_button >= NUM_BUTTONS)
        res = -1;

        else
        {
            uint8_t mask = _pinrefs[which_button].mask;

            if(_pinrefs[which_button].port_is_odd)
            {
                val = _pinrefs[which_button].odd->IFG & _pinrefs[which_button].mask;
            }

            else
            {
                val = _pinrefs[which_button].even->IFG & _pinrefs[which_button].mask;
            }

            if(val == 0)
                res = 0;
            else
            {
                res = 1;
                if(_pinrefs[which_button].port_is_odd)
                    {
                        _pinrefs[which_button].odd->IFG = _pinrefs[which_button].odd->IFG & ~(_pinrefs[which_button].mask);
                    }

                else
                    {
                        _pinrefs[which_button].even->IFG = _pinrefs[which_button].even->IFG & ~(_pinrefs[which_button].mask);
                    }
            }
        }

        return res;
}
