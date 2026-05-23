/*
 * main.c - Part 3.2: Blinky - Solution
 *
 * Strategy:
 *   Call blink() or zwei_leds_blinken_lassen() from the main loop.
 *   Both functions use a global volatile counter to implement a
 *   busy-wait delay without blocking the CPU in a single long loop.
 *
 *   volatile tells the compiler not to optimise away the counter —
 *   without it, the compiler may detect that the loop has no visible
 *   side effects and remove it entirely.
 *
 *   OUTTGL flips the state of selected pins in a single instruction,
 *   which is more efficient than reading, masking, and writing back.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"

int main(void)
{
    PORTD.DIRSET = PIN7_bm;   /* configure PD7 as output */
    PORTD.OUTSET = PIN7_bm;   /* start with LED on */

    while (1)
    {
        /* Part 3.2-a: single LED blink */
        blink();

        /* Part 3.2-b: uncomment below and comment above to test two LEDs */
        /* zwei_leds_blinken_lassen(); */
    }
}
