/*
 * main.c Part 6.2: Timer
 *
 * Display a seconds counter on the LCD using the TCA0 timer interrupt.
 * The counter increments exactly once per second.
 *
 * Timer config: F_CPU = 4 MHz, prescaler = 64, PER = 62500 -> 1 Hz overflow
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

/* YOUR CODE HERE - declare shared variables */

/* YOUR CODE HERE - implement int_to_string() */

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE */
    /* Remember to clear the interrupt flag at the end */
}

int main(void)
{
    lcd_init();

    /* YOUR CODE HERE, configure TCA0 */

    sei();

    while (1) {
		
	}
}