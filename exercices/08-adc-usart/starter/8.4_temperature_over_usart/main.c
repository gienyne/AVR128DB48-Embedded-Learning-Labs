/*
 * main.c Part 8.4: Internal Temperature Readings over USART
 *
 * Read the internal temperature sensor and transmit readings over USART.
 * Use TCA0 for the 1-second interval. Output format:
 *   "Time: N s, Temp: XX.X degC, YYY.Y K\n"
 *
 * Hardware:
 *   USART3 TX -> PB0
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD_RATE 9600

/* YOUR CODE HERE */

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