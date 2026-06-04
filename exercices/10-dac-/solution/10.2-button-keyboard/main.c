/*
 * main.c Part 10.2: Button Keyboard
 *
 * Strategy:
 *   Each button press starts a note by setting TCA0.SINGLE.PER to the
 *   period corresponding to that note's frequency and enabling the timer.
 *   Releasing all buttons stops the note by disabling the timer.
 *
 * BOTHEDGES triggers the ISR on both press and release.
 * Inside the ISR, PORTA.IN is read to distinguish
 * a pressed button (pin HIGH) from a released button (pin LOW).
 * The buttons are externally pulled down by the hardware.
 * If no button is HIGH, stop_note() is executed.
 *
 *   PD6 digital input is disabled to reduce noise on the DAC output pin.
 *
 *   Note: only one note can play at a time since a single timer drives the DAC.
 *   If multiple buttons are pressed simultaneously, priority follows the order of the if/else chain: C > D > E > F > G > H.
 *
 * Hardware:
 *   PA2–PA7 -> buttons C, D, E, F, G, H
 *   PB0     -> button A
 *   PD6     -> DAC output -> amplifier -> speaker
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

void DAC_init(void)
{
    VREF.DAC0REF   = VREF_REFSEL_2V048_gc;
    PORTD.PIN6CTRL = PORT_ISC_INPUT_DISABLE_gc; // reduce noise on DAC pin 
    DAC0.CTRLA     = DAC_ENABLE_bm | DAC_OUTEN_bm;
    DAC0.DATA      = sine_table[0] << 6;
}

void button_init(void)
{
    PORTA.DIRCLR   = PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTB.DIRCLR   = PIN0_bm;

    PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN3CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN5CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN6CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTA.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
    PORTB.PIN0CTRL = PORT_ISC_BOTHEDGES_gc;
}

void timer_init(void)
{
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV1_gc;
}

void start_note(uint16_t freq)
{
    sine_index = 0;
    TCA0.SINGLE.PER   = TCA_PER(freq);
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void stop_note(void)
{
    TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}

ISR(PORTA_PORT_vect)
{
    uint8_t state = PORTA.IN;

    if (state & PIN2_bm) 
	{
		start_note(c);
	}
    else if (state & PIN3_bm)
	{
		start_note(d);
	}
    else if (state & PIN4_bm)
	{
		start_note(e);
	}
    else if (state & PIN5_bm)
	{
		start_note(f);
	}
    else if (state & PIN6_bm)
	{
		start_note(g);
	}
    else if (state & PIN7_bm)
	{
		start_note(h);
	}
    else
		{
		stop_note();
	}

    PORTA.INTFLAGS = PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
}

ISR(PORTB_PORT_vect)
{
    if (PORTB.IN & PIN0_bm) 
	{
		start_note(a);
	}
    else 
	{
		stop_note();
	}

    PORTB.INTFLAGS = PIN0_bm;
}

ISR(TCA0_OVF_vect)
{
    DAC0.DATA = sine_table[sine_index] << 6;

    sine_index++;
    if (sine_index >= SINE_SIZE) {
		sine_index = 0;
	}

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void)
{
    button_init();
    DAC_init();
    timer_init();
    sei();

    while (1) {
		
	}
}
