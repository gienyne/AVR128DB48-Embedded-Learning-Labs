/*
 * main.c Part 7.3: Waving Servomotor - Solution
 *
 * Strategy:
 *   Unlike LED PWM, the servo motor responds to the absolute duration of the
 *   HIGH pulse within a fixed 20 ms period, not the duty cycle percentage.
 *
 *   Timer configuration (F_CPU = 4 MHz, prescaler DIV8):
 *     tick rate = 4,000,000 / 8 = 500,000 ticks/s
 *     20 ms period -> PER = 500,000 * 0.020 = 10,000
 *
 *   Pulse width to position mapping:
 *     1.0 ms -> 500  ticks -> full left
 *     1.5 ms -> 750  ticks -> center
 *     2.0 ms -> 1000 ticks -> full right
 *
 *   Smooth movement is achieved by incrementing CMP0 by a small step (10 ticks)
 *   with a short delay (10 ms) between each update, rather than jumping directly
 *   between positions. Smaller steps and shorter delays produce faster movement.
 *
 *   The servo is connected to PF4. TCA0 CMP0 is routed to Port F via PORTMUX.
 *   Note: on Port F the CMP0 output appears on PF0 by default, but the physical
 *   wiring here uses PF4 , verify pin assignment against the board schematic.
 *
 * Hardware:
 *   Servo signal -> PF4
 *   Servo power  -> 5V rail
 *   Servo ground -> GND rail
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PWM_PERIOD    10000   /* 20 ms at 500,000 ticks/s */
#define PULSE_LEFT    500     /* 1.0 ms */
#define PULSE_CENTER  750     /* 1.5 ms */
#define PULSE_RIGHT   1000    /* 2.0 ms */
#define STEP          10      /* ticks per increment: controls sweep speed */
#define STEP_DELAY    10      /* ms between steps: controls sweep speed */

void init_pwm(void)
{
    PORTF.DIRSET = PIN4_bm;

    /* route TCA0 to Port F */
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTF_gc;

    /* enable CMP0 output, Single Slope PWM */
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                      | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    TCA0.SINGLE.PER  = PWM_PERIOD;
    TCA0.SINGLE.CMP0 = PULSE_CENTER;   /* start at center position */

    /* prescaler DIV8, start timer */
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc
                      | TCA_SINGLE_ENABLE_bm;
}

void set_servo_position(uint16_t ticks)
{
    TCA0.SINGLE.CMP0 = ticks;
}

/*
 * sweep()
 *
 * Moves the servo smoothly from full left to full right, then back.
 * Each step changes CMP0 by STEP ticks, with STEP_DELAY ms between steps.
 */
void sweep(void)
{
    int16_t pos;

    /* left -> right */
    for (pos = PULSE_LEFT; pos <= PULSE_RIGHT; pos += STEP) {
        set_servo_position((uint16_t)pos);
        _delay_ms(STEP_DELAY);
    }

    /* right -> left */
    for (pos = PULSE_RIGHT; pos >= PULSE_LEFT; pos -= STEP) {
        set_servo_position((uint16_t)pos);
        _delay_ms(STEP_DELAY);
    }
}

int main(void)
{
    init_pwm();

    /* Phase 1: center -> left -> right, 1 second each */
    set_servo_position(PULSE_CENTER);
    _delay_ms(1000);
    set_servo_position(PULSE_LEFT);
    _delay_ms(1000);
    set_servo_position(PULSE_RIGHT);
    _delay_ms(1000);

    /* Phase 2: continuous sweep */
    while (1)
    {
        sweep();
    }
}