/*
 * main.c Part 8.5: RGB LED Control over USART
 *
 * Receive RGB values over USART and set the LED color.
 * Protocol: send "r,g,b." , dot terminates each message.
 *
 * Hardware:
 *   RGB LED: PE0 (Red), PE1 (Green), PE2 (Blue)
 *   USART3: PB0 TX, PB1 RX
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE 9600

/* YOUR CODE HERE */

ISR(USART3_RXC_vect)
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