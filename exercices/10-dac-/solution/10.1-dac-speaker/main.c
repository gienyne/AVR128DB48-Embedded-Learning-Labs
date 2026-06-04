/*
 * main.c Part 10.1: D/A Conversion with Speaker
 *
 * Strategy:
 *   The DAC outputs one sample from sine_table[] on each TCA0 overflow.
 *   The timer period determines how fast the table is stepped through,
 *   which sets the output frequency.
 *
 *   Frequency formula:
 *     f = F_CPU / ((PER + 1) * SINE_SIZE)
 *     PER = (F_CPU / (f * SINE_SIZE)) - 1
 *
 *   For A4 (440 Hz), SINE_SIZE = 64, F_CPU = 4 MHz:
 *     PER = (4,000,000 / (440 * 64)) - 1 = 141 - 1 = 140
 *
 *   DAC register alignment:
 *     DAC0.DATA is a 16-bit register. Only bits 15:6 are used (10 data bits).
 *     sine_table values are 10-bit (0–1023). Left-shifting by 6 places them
 *     in the correct bit positions of the DATA register.
 *
 * Hardware:
 *   DAC output -> PD6 -> PAM8302A amplifier -> speaker
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../lib/song.h"

#define SINE_SIZE  64
#define F_SIGNAL   a1   /* 440 Hz */

volatile uint8_t sine_index = 0;

void DAC_init(void)
{
    VREF.DAC0REF = VREF_REFSEL_2V048_gc;   // 2.048V reference
    DAC0.CTRLA   = DAC_ENABLE_bm | DAC_OUTEN_bm; // enable DAC
    DAC0.DATA    = sine_table[0] << 6; // initial value
}

void timer_init(void)
{
    uint16_t period = (uint16_t)(F_CPU / ((uint32_t)F_SIGNAL * SINE_SIZE)) - 1;

    TCA0.SINGLE.PER     = period;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

ISR(TCA0_OVF_vect)
{
    DAC0.DATA = sine_table[sine_index] << 6;

    sine_index++;
    if (sine_index >= SINE_SIZE) sine_index = 0;

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void)
{
    DAC_init();
    timer_init();
    sei();

    while (1) {
		
	}
}
