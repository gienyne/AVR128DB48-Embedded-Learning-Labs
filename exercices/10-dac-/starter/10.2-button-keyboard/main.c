/*
 * main.c Part 10.2: Button Keyboard
 *
 * Play a note while a button is held. Release stops the sound.
 *
 * Button mapping:
 *   PA2=C, PA3=D, PA4=E, PA5=F, PA6=G, PA7=H, PB0=A
 *
 * Use BOTHEDGES interrupts. Check PORTA.IN inside the ISR
 * to distinguish press (HIGH) from release (LOW).
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../lib/song.h"

#define SINE_SIZE 64
#define TCA_PER(freq) ((uint16_t)((F_CPU / ((uint32_t)(freq) * SINE_SIZE)) - 1))

volatile uint8_t sine_index = 0;

/* YOUR CODE HERE */

ISR(PORTA_PORT_vect)
{
    /* YOUR CODE HERE */
}

ISR(PORTB_PORT_vect)
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