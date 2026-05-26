/*
 * main.c Part 7.1: Dimming Red LED
 *
 * Light the red LED on PD0 at 25% brightness using hardware PWM.
 * Implement set_brightness(percent) to control the duty cycle.
 *
 * Hardware:
 *   Red LED -> PD0 via series resistor
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>

#define BRIGHTNESS 25   /* percent: change this to test other levels */

void set_brightness(uint8_t percent)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* YOUR CODE HERE */

    set_brightness(BRIGHTNESS);

    while (1) {
		
	}
}