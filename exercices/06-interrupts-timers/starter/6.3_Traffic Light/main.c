/*
 * main.c Part 6.3: Traffic Light
 *
 * Implement a traffic light state machine using button interrupts and TCA0 timer.
 *
 * States: ROT (red), GRUEN (green), GELB2ROT (transition to red), GELB2GRUEN (transition to green)
 *
 * Hardware note: blue (PE2) is used in place of yellow. The RGB LED
 * used here has no native yellow output.
 *
 * Hardware:
 *   RGB LED -> PE0 (Red), PE1 (Green), PE2 (Blue / Yellow substitute)
 *   Button PC4 -> RED to GREEN transition
 *   Button PC5 -> GREEN to RED transition
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/* YOUR CODE HERE - define states */

ISR(PORTC_PORT_vect)
{
    /* YOUR CODE HERE */
}

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* YOUR CODE HERE */

    sei();

    while (1) {
	}
	
}