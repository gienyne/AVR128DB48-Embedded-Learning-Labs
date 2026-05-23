/*
 * main.c - Part 5.2: Count Up
 *
 * Count from 0 to 1000 and display each value on the LCD.
 * Use a delay of ~1.5 seconds between each number.
 *
 * You will need to write a helper function to convert integers to strings,
 * since lcd_putString() expects a char*.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"

/* Hint: allocate 11 bytes. Max 10 digits + null terminator */
#define BUFFER_SIZE 11

/* YOUR CODE HERE - implement int_to_string() */

int main(void)
{
    lcd_init();

    while (1)
    {
        /* YOUR CODE HERE */
    }
}