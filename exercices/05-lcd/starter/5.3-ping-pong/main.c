/*
 * main.c Part 5.3: Ping Pong
 *
 * Move a single '1' across both rows of the LCD:
 * left to right on row 0, then right to left on row 1.
 *
 * Constraint: do not use any pre-built LCD movement functions.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"

int main(void)
{
    lcd_init();

    while (1)
    {
        /* YOUR CODE HERE */
    }
}