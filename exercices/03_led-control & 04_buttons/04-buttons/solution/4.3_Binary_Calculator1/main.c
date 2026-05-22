/*
 * main.c - Part 4.3: Binary Calculator - Solution
 *
 * Strategy:
 *   Binary_calculator() reads all four buttons at once and uses edge
 *   detection to act only on new presses, not held buttons.
 *   Comparing current state with previous state avoids repeated operations.
 *   A 30 ms delay at the end of each loop debounces all four buttons.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"

int main(void)
{
    Binary_calculator();

    while (1) {
		
	}
}