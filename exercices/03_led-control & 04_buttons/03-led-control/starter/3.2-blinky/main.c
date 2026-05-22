/*
 * main.c - Part 3.2: Blinky
 *
 * Make one LED (PD7) blink by toggling its state repeatedly with a delay.
 * Extend to two LEDs (PD6 and PD7) blinking simultaneously.
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
        /* blink(); */

        /* Part 3.2-b: two LEDs blinking together */
        /* zwei_leds_blinken_lassen(); */
    }
}
