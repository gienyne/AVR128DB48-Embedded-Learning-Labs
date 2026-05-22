/*
 * main.c - Part 3.4: Binary Counter - Solution
 *
 * Strategy:
 *   binary_counter() counts from 0 to 255 and writes each value
 *   directly to PORTD.OUT. Because each bit of the register maps
 *   directly to one LED pin, no bit manipulation is needed -
 *   the hardware does the mapping automatically.
 *
 *   A uint8_t variable naturally holds values 0–255 and wraps
 *   back to 0 on overflow, which matches the 8-LED display range.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"

int main(void)
{
    while (1)
    {
        binary_counter();
    }
}