/*
 * main.c - Part 3.3: Moving Lights - Solution
 *
 * Strategy:
 *   Lauflicht() shifts a single lit LED from PD0 to PD7, then back.
 *   Writing (PIN0_bm << i) to PORTD.OUT sets exactly one pin HIGH
 *   and clears all others in a single register write - no need to
 *   manually clear the previous LED before lighting the next one.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"

int main(void)
{
    while (1)
    {
        Lauflicht();
    }
}