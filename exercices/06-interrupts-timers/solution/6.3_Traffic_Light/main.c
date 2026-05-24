/*
 * main.c Part 6.3: Traffic Light - Solution
 *
 * Strategy:
 *   A state machine with four states controls the RGB LED:
 *     ROT        -> red LED on (PE0)
 *     GRUEN      -> green LED on (PE1)
 *     GELB2ROT   -> blue LED on (PE2), transitioning to red
 *     GELB2GRUEN -> blue LED on (PE2), transitioning to green
 *
 *   Hardware note: the RGB LED used here has no native yellow output.
 *   Blue (PE2) is used in place of yellow as a hardware limitation.
 *   The state machine logic is otherwise identical to a real traffic light.
 *
 *   Button ISR (PORTC_PORT_vect):
 *     PC4 -> if not already in a transition, set GELB2GRUEN and delay = 2000
 *     PC5 -> if not already in a transition, set GELB2ROT   and delay = 2000
 *     TCA0 counter is reset on each button press to start the delay cleanly.
 *
 *   Timer ISR (TCA0_OVF_vect):
 *     Each overflow = 1 ms (PER adjusted accordingly).
 *     Decrements delay on each tick. When delay reaches 0, applies the final state.
 *     Timer is disabled after the transition completes to save power.
 *
 *   Separation of concerns:
 *     Button ISR  -> detects events and initiates transitions
 *     Timer ISR   -> manages timing and completes transitions
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define ROT        0
#define GRUEN      1
#define GELB2ROT   2
#define GELB2GRUEN 3

volatile int zustand = ROT;
volatile int delay   = 0;

ISR(PORTC_PORT_vect)
{
    if (PORTC.INTFLAGS & PIN4_bm)
    {
        /* RED -> GREEN transition: only if not already transitioning */
        if (zustand != GELB2GRUEN) {
            PORTE.OUT = PIN2_bm;    /* blue substitutes yellow */
            zustand   = GELB2GRUEN;
            delay     = 2000;       /* 2000 ms yellow phase */
            TCA0.SINGLE.CNT   = 0;
            TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
        }
        PORTC.INTFLAGS = PIN4_bm;
    }

    if (PORTC.INTFLAGS & PIN5_bm)
    {
        /* GREEN -> RED transition */
        if (zustand != GELB2ROT) {
            PORTE.OUT = PIN2_bm;
            zustand   = GELB2ROT;
            delay     = 2000;
            TCA0.SINGLE.CNT   = 0;
            TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
        }
        PORTC.INTFLAGS = PIN5_bm;
    }
}

ISR(TCA0_OVF_vect)
{
    if (delay > 0) {
        delay--;
    }
    else {
        /* yellow (blue to be more precise) phase complete, apply final state */
        switch (zustand) {
            case GELB2GRUEN:
                PORTE.OUT = PIN1_bm;   /* green on */
                zustand   = GRUEN;
                break;
            case GELB2ROT:
                PORTE.OUT = PIN0_bm;   /* red on */
                zustand   = ROT;
                break;
            default:
                break;
        }
        /* disable timer until next button press */
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
    }

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   /* clear overflow flag */
}

int main(void)
{
    /* rising edge interrupt */
    PORTC.DIRCLR   = PIN4_bm | PIN5_bm;
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;
    PORTC.PIN5CTRL = PORT_ISC_RISING_gc;

    /* RGB LED outputs: start with red */
    PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
    PORTE.OUT    = PIN0_bm;

    TCA0.SINGLE.PER = 62500; // Périodedauer für 1 sekunde (für einen zähler von 4 MHz / 64)
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL0_bm | TCA_SINGLE_CLKSEL2_bm;
    /* timer starts disabled — enabled on first button press */

    sei();

    while (1) {}
}