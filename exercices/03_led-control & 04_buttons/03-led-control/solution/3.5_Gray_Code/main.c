/*
 * main.c - Part 3.5: Gray Code - Solution
 *
 * Strategy:
 *   gray_code() counts from 0 to 255 but converts each value to its
 *   Gray code equivalent before writing to PORTD.OUT.
 *
 *   Gray code formula: gray = binary ^ (binary >> 1)
 *
 *   The key property of Gray code is that consecutive values differ
 *   by exactly one bit - meaning only one LED changes state at each step.
 *   This makes it useful in applications where multiple simultaneous
 *   transitions could cause errors (encoders, state machines).
 *
 *   Example:
 *     binary = 3 (011) -> gray = 3 ^ 1 = 2 (010)
 *     binary = 4 (100) -> gray = 4 ^ 2 = 6 (110)
 *     Only one bit differs between consecutive gray values.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"
#include "../../shared/leds.c"

int main(void)
{
    while (1)
    {
        gray_code();
    }
}
