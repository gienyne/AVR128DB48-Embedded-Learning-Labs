/*
 * main.c - Part 4.2: Debounced Counter - Solution
 *
 * Strategy:
 *   Debounced_Counter() detects the first falling edge on PC4,
 *   waits 30 ms, then confirms the press before incrementing.
 *   It waits for button release before accepting the next press.
 *   This ensures one physical press = exactly one increment.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../shared/leds.h"
#include "../../shared/leds.c"

int main(void)
{
    Debounced_Counter();

    while (1) {}
}
