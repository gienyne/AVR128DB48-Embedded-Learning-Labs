/*
 * main.c Part 6.1: Button Interrupt
 *
 * Toggle the RGB LED on each button press using an ISR.
 * Generate a new random color on each activation using Xorshift32.
 *
 * Hardware:
 *   RGB LED -> PE0 (Red), PE1 (Green), PE2 (Blue)
 *   Button  -> PC4
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define RGB_LED (PIN0_bm | PIN1_bm | PIN2_bm)

/* YOUR CODE HERE */

ISR(PORTC_PORT_vect)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* Configure RGB LED pins as output */
    PORTE.DIRSET = RGB_LED;

    /* External pull-down present, interrupt on rising edge */
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;

    /* Enable global interrupts */
    sei();

    while (1) {
		
	}
}