/*
 * main.c Part 8.2: A/D Conversion with Photoresistor
 *
 * Read the analog voltage from the photoresistor on PF2 (AIN18).
 * Display voltage in mV and light level as percentage on the LCD.
 * Calibrate the 100% upper bound dynamically at runtime.
 *
 * Hardware:
 *   Photoresistor voltage divider -> PF2 (AIN18)
 *   LCD via I2C (PA2 SDA, PA3 SCL)
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

/* YOUR CODE HERE */

int main(void)
{
    /* YOUR CODE HERE */
    while (1) {
		
	}
}