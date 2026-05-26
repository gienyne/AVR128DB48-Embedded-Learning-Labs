/*
 * main.c Part 7.2: Gradient Rainbow LED - Solution
 *
 * Strategy:
 *   Three TCA0 compare channels (CMP0, CMP1, CMP2) drive the three RGB channels
 *   independently. Each channel's duty cycle controls that color's brightness.
 *
 * Hardware:
 *   RGB LED: PE0 = Red (CMP0), PE1 = Green (CMP1), PE2 = Blue (CMP2)
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PWM_TOP 100

void set_rgb_brightness(uint8_t red, uint8_t green, uint8_t blue)
{
    if (red   > 100) red   = 100;
    if (green > 100) green = 100;
    if (blue  > 100) blue  = 100;

    TCA0.SINGLE.CMP0 = (red   * PWM_TOP) / 100;
    TCA0.SINGLE.CMP1 = (green * PWM_TOP) / 100;
    TCA0.SINGLE.CMP2 = (blue  * PWM_TOP) / 100;
}

void rainbow_transition(void)
{
    uint8_t i;

    for (i = 0; i <= 100; i++) { set_rgb_brightness(100, i,   0);   _delay_ms(30); } /* red -> yellow  */
    for (i = 100; i > 0; i--) { set_rgb_brightness(i,   100, 0);   _delay_ms(30); } /* yellow -> green */
    for (i = 0; i <= 100; i++) { set_rgb_brightness(0,   100, i);   _delay_ms(30); } /* green -> cyan   */
    for (i = 100; i > 0; i--) { set_rgb_brightness(0,   i,   100); _delay_ms(30); } /* cyan -> blue    */
    for (i = 0; i <= 100; i++) { set_rgb_brightness(i,   0,   100); _delay_ms(30); } /* blue -> magenta */
    for (i = 100; i > 0; i--) { set_rgb_brightness(100, 0,   i);   _delay_ms(30); } /* magenta -> red  */
}

int main(void)
{
    PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;

    /* route TCA0 PWM output to Port E: CMP0->PE0, CMP1->PE1, CMP2->PE2 */
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc;

    /* enable all three compare channels, Single Slope PWM mode */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                      | TCA_SINGLE_CMP1EN_bm
                      | TCA_SINGLE_CMP2EN_bm
                      | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    TCA0.SINGLE.PER   = PWM_TOP;

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc
                      | TCA_SINGLE_ENABLE_bm;

    while (1)
    {
        rainbow_transition();
    }
}