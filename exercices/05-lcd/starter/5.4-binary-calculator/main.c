/*
 * main.c Part 5.4: Binary Calculator II
 *
 * Extend the Binary Calculator from Exercise 04 (Part 4.3).
 * Display the result in binary on the LEDs and in decimal on the LCD.
 * Handle negative values correctly on the LCD.
 *
 * Button assignments:
 *   PC4 -> increment (+1)
 *   PC5 -> decrement (-1)
 *   PC6 -> left shift (*2)
 *   PC7 -> right shift (/2)
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"
#include "../../shared/leds.h"

/* YOUR CODE HERE - implement int_to_string() with negative number support */

int main(void)
{
    lcd_init();

    while (1)
    {
        /* YOUR CODE HERE */
    }
}