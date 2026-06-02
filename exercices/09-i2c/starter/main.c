/*
 * main.c Part 9.1: Color Sensor (TCS34725)
 *
 * Read RGBC color values from the TCS34725 sensor and display them on the LCD.
 *
 * Initialization sequence:
 *   ENABLE register (0x00): 0x03  -> Power ON + ADC Enable
 *   ATIME  register (0x01): 0xD5  -> Integration time circa 103 ms
 *   WTIME  register (0x03): 0xFF  -> Wait time between measurements
 *
 * Note: every register address must be ORed(|) with CMD_BIT (0x80).
 *
 * Hardware:
 *   TCS34725: PA2 (SDA), PA3 (SCL)
 *   LCD:      PA2 (SDA), PA3 (SCL): shared I²C bus, address 0x27
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

#define TCS34725_ADDRESS  0x29
#define ENABLE_REGISTER   0x00
#define ATIME_REGISTER    0x01
#define WTIME_REGISTER    0x03
#define CMD_BIT           0x80

/* YOUR CODE HERE, declare global variables */

void sensor_init(void)
{
    /* YOUR CODE HERE */
}

uint16_t merge_bytes(uint8_t low, uint8_t high)
{
    /* YOUR CODE HERE */
    return 0;
}

void read_rgbc(void)
{
    /* YOUR CODE HERE */
}

void display_rgbc(void)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    i2c_init();
    lcd_init();
    lcd_enable(true);

    sensor_init();

    while (1)
    {
        read_rgbc();
        display_rgbc();
        _delay_ms(500);
    }
}