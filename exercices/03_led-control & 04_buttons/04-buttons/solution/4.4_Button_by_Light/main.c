/*
 * main.c - Part 4.4: Button by Light - Solution
 *
 * Strategy:
 *   button_by_light() selects a random index (0–3) using rand() % 4,
 *   then shifts PIN4_bm left by that index to pick one of PD4–PD7.
 *   The matching button shares the same pin number on PORTC.
 *   The inner loop polls PORTC.IN until the correct button is pressed.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"
#include "../../shared/leds.c"

int main(void)
{
    button_by_light();

    while (1) {
		
	}
}
