/*
 * main.c - Part 5.1: Hello Display
 *
 * Write "Hello Display" on the bottom row of the LCD.
 *
 * Target: AVR128DB48
 * LCD:    HD44780 1602 via HW-061 I2C module (address 0x27)
 * I2C:    SDA -> PA2, SCL -> PA3
 */

#include <avr/io.h>
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