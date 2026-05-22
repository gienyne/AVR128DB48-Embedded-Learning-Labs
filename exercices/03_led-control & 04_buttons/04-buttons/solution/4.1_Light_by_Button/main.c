/*
 * main.c - Part 4.1: Light By Button - Solution
 *
 * Strategy:
 *   Configure PD0 as output and PC4 as input with internal pull-up.
 *   Buttons are active-low: pin reads HIGH when open, LOW when pressed.
 *   Poll PORTC.IN in a loop and set or clear the LED accordingly.
 *
 *   light_by_bouton_a() handles the single button/LED case.
 *   light_by_bouton_b() handles all buttons/LEDs simultaneously.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"
#include "../../shared/leds.c"

int main(void)
{
    /* Part 4.1-a: single button controls single LED */
    light_by_bouton_a();

    /* Part 4.1-b: any button controls all LEDs. Uncomment to test */
    /* light_by_bouton_b(); */

    while (1) {
		
	}
}
