/*
 * main.c Part 6.4: Programmable Timer - Solution
 *
 * Strategy:
 *   Two ISRs cooperate to implement the programmable countdown:
 *
 *   PORTC_PORT_vect (button ISR):
 *     PC4 -> sets pin4_interrupt flag; time is added inside the timer ISR,
 *            not here — this keeps the button ISR short and avoids race conditions.
 *     PC5 -> toggles the timer enable bit to pause/resume the countdown.
 *            The pause is only allowed while the timer is still running (timer > 0).
 *
 *   TCA0_OVF_vect (timer ISR, 1 overflow per second):
 *     1. Check pin4_interrupt flag: if set, add 5 seconds and clear the flag.
 *     2. Decrement timer if > 0.
 *     3. If timer == 0: light up the red LED and disable the timer.
 *     4. Update LCD with current remaining time.
 *     5. Clear the overflow flag.
 *
 *   Key design decision: time addition is handled inside the timer ISR, not in
 *   the button ISR. This avoids modifying `timer` from two different ISRs
 *   simultaneously, which could cause a race condition on 16-bit reads/writes.
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

volatile uint16_t timer        = 10;   /* countdown in seconds, starts at 10 */
volatile uint8_t  pin4_interrupt = 0;  /* flag set by PC4 ISR                */
char convert[6];

char* int_to_string(char* buffer, uint16_t value)
{
    int pos = 0;

    if (value == 0) {
        buffer[pos++] = '0';
    }

    while (value > 0) {
        buffer[pos++] = '0' + (value % 10);
        value /= 10;
    }

    buffer[pos] = '\0';

    for (int i = 0; i < pos / 2; i++) {
        char tmp        = buffer[i];
        buffer[i]       = buffer[pos - 1 - i];
        buffer[pos-1-i] = tmp;
    }

    return buffer;
}

ISR(PORTC_PORT_vect)
{
    if (PORTC.INTFLAGS & PIN4_bm) {
        pin4_interrupt = 1;           /* signal the timer ISR to add time */
        PORTC.INTFLAGS = PIN4_bm;
    }

    if (PORTC.INTFLAGS & PIN5_bm) {
        if (timer > 0) {
            /* toggle timer running state , pause or resume */
            TCA0.SINGLE.CTRLA ^= TCA_SINGLE_ENABLE_bm;
        }
        PORTC.INTFLAGS = PIN5_bm;
    }
}

ISR(TCA0_OVF_vect)
{
    /* handle +5s request from PC4 button */
    if (pin4_interrupt) {
        timer += 5;
        PORTE.OUTCLR = PIN0_bm;   /* turn off red LED if it was on */
        pin4_interrupt = 0;
    }

    /* countdown */
    if (timer > 0) {
        timer--;
    }

    if (timer == 0) {
        PORTE.OUTSET = PIN0_bm;                        /* red LED on, time expired */
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;   /* stop timer                */
    }

    /* update LCD */
    lcd_clear();
    lcd_putString(int_to_string(convert, timer));

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   /* clear overflow flag */
}

int main(void)
{
    PORTC.DIRCLR   = PIN4_bm | PIN5_bm;
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;
    PORTC.PIN5CTRL = PORT_ISC_RISING_gc;

    PORTE.DIRSET = PIN0_bm;   /* red LED output */

    lcd_init();

    TCA0.SINGLE.PER     = 62500;  /* 1 s overflow at 4 MHz / prescaler 64 */
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm
                        | TCA_SINGLE_CLKSEL0_bm
                        | TCA_SINGLE_CLKSEL2_bm;

    sei();

    while (1) {
	}
	
}