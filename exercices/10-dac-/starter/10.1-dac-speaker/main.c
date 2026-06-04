/*
 * main.c Part 10.1: D/A Conversion with Speaker
 *
 * Generate a continuous sine wave at 440 Hz (A4) through the speaker.
 * Use sine_table[] from song.h and a TCA0 overflow interrupt.
 *
 * Timer period formula:
 *   PER = (F_CPU / (frequency * SINE_SIZE)) - 1
 *
 * DAC register note:
 *   DAC0.DATA is 16-bit. Bits 15:6 carry the 10-bit value.
 *   Left-shift sine_table values by 6 before writing.
 *
 * Hardware:
 *   DAC output -> PD6 -> amplifier -> speaker
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../lib/song.h"

#define SINE_SIZE 64
#define F_SIGNAL  a1   /* 440 Hz */

volatile uint8_t sine_index = 0;

void DAC_init(void)
{
    /* YOUR CODE HERE */
}

void timer_init(void)
{
    /* YOUR CODE HERE */
}

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    DAC_init();
    timer_init();
    sei();
    while (1) {
		
	}
}