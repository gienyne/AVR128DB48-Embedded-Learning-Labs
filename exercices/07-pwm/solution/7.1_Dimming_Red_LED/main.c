/*
 * main.c Part 7.1: Dimming Red LED - Solution
 *
 * Strategy:
 *   TCA0 runs in Single Slope PWM mode. The timer counts from 0 to PER (100),
 *   then resets. CMP0 defines when the output switches from HIGH to LOW:
 *
 *     counter < CMP0  -> pin HIGH (LED on)
 *     counter >= CMP0 -> pin LOW  (LED off)
 *
 *   set_brightness() converts a percentage to a compare value:
 *     CMP0 = (percent * PER) / 100
 *
 *   At 25%: CMP0 = 25 -> LED is on for 25 out of every 100 ticks.
 *
 *   PORTMUX routes the TCA0 CMP0 output to PD0.
 *   Without this, the PWM signal exists internally but never reaches the pin.
 *
 * Hardware:
 *   Red LED -> PD0 via series resistor
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>

#define BRIGHTNESS 25   /* percent: change this to test other levels */

void set_brightness(uint8_t percent)
{
    if (percent > 100) percent = 100;

    /* convert percentage to compare value relative to PER */
    TCA0.SINGLE.CMP0 = (percent * TCA0.SINGLE.PER) / 100;
}

int main(void)
{
    PORTD.DIRSET = PIN0_bm;   /* PD0 as output */

    /* route TCA0 PWM output to Port D: CMP0 -> PD0 */
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;

    /* enable CMP0 output and select Single Slope PWM mode */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                      | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    /* PWM cycle length: timer counts 0..100 */
    TCA0.SINGLE.PER = 100;

    /* prescaler DIV4: 4,000,000 / 4 = 1,000,000 ticks/s
     * PWM frequency: 1,000,000 / 101 = 10 kHz              */
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc
                      | TCA_SINGLE_ENABLE_bm;

    set_brightness(BRIGHTNESS);

    while (1) {
		
	}
}