/*
 * main.c Part 7.3: Waving Servomotor
 *
 * Phase 1: move to center, then full left, then full right (1 second each).
 * Phase 2: sweep continuously left to right and back.
 *
 * Pulse width reference (DIV8, 4 MHz -> 500,000 ticks/s):
 *   Full left  = 1.0 ms = 500  ticks
 *   Center     = 1.5 ms = 750  ticks
 *   Full right = 2.0 ms = 1000 ticks
 *   Period     = 20 ms  = 10000 ticks
 *
 * Hardware:
 *   Servo signal -> PF4
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

/* YOUR CODE HERE: define pulse width constants */

void init_pwm(void)
{
    /* YOUR CODE HERE */
}

void set_servo_position(uint16_t ticks)
{
    /* YOUR CODE HERE */
}

void sweep(void)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    init_pwm();

    /* Phase 1 */
    /* YOUR CODE HERE */

    /* Phase 2 */
    while (1)
    {
        sweep();
    }
}