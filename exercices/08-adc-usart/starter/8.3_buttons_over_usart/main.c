/*
 * main.c Part 8.3: Pressed Buttons over USART
 *
 * Send a character over USART each time a button is pressed:
 *   PC4 -> 'K', PC5 -> 'A', PC6 -> 'M', PC7 -> 'U' : you can also use other charakters
 *
 * Use a single ISR for all four buttons.
 *
 * Hardware:
 *   Buttons -> PC4, PC5, PC6, PC7
 *   USART3 TX -> PB0
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE 9600

/* YOUR CODE HERE */

ISR(PORTC_PORT_vect)
{
    /* YOUR CODE HERE */
}
