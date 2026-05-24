/*
 * main.c Part 6.4: Programmable Timer
 *
 * Countdown timer displayed on the LCD.
 *   - Starts at 10 seconds
 *   - PC4 adds 5 seconds to the remaining time
 *   - PC5 pauses and resumes the countdown
 *   - When timer reaches 0: RGB LED turns red, timer stops
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

ISR(PORTC_PORT_vect)
{
    /* YOUR CODE HERE */
}

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE */
    /* Remember to clear the interrupt flag at the end */
}

int main(void)
{
    lcd_init();

    /* YOUR CODE HERE - configure pins, timer, interrupts */

    sei();

    while (1) {
		
	}
}