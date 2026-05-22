/*
 * main.c - Part 3.1: Light Up - Solution
 *
 * Strategy:
 *   Configure PD7 as an output using DIRSET.
 *   Drive it HIGH using OUTSET.
 *   The LED stays on permanently.
 *
 * DIRSET writes a 1 to the direction register for each bit that is set,
 * configuring those pins as outputs without affecting the others.
 * OUTSET drives selected pins HIGH without affecting others.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"

int main(void)
{
    PORTD.DIRSET = PIN7_bm;   /* set PD7 as output */
    PORTD.OUTSET = PIN7_bm;   /* drive PD7 HIGH - LED on */

    while (1)
    {
        /* nothing to do - LED stays on */
    }
}

