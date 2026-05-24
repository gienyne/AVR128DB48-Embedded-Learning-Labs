/*
 * main.c Part 6.1: Button Interrupt - Solution
 *
 * Strategy:
 *   A single ISR on PORTC_PORT_vect handles the PC4 button press.
 *   led_state toggles between 0 (OFF) and 1 (ON) on each press.
 *
 *   Xorshift32 is a lightweight pseudo-random number generator that uses
 *   XOR and bit shift operations on a 32-bit state variable.
 *   It is deterministic: the same seed always produces the same sequence.
 *   The seed can be changed to get a different sequence.
 *
 *   The random value is masked with RGB_LED (0b00000111) to extract
 *   3 bits, one per color channel. The do/while loop rejects 0x00
 *   to ensure at least one LED channel is always active.
 *
 *   Interrupt flag must be cleared manually inside the ISR
 *   the hardware sets it but never clears it automatically.
 *
 * Hardware:
 *   RGB LED -> PE0 (Red), PE1 (Green), PE2 (Blue)
 *   Button  -> PC4
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define RGB_LED (PIN0_bm | PIN1_bm | PIN2_bm)

volatile uint8_t led_state      = 0;       /* 0 = OFF, 1 = ON            */
uint32_t         xorshift_state = 0x16;    /* initial seed, change freely */

/*
 * xorshift32()
 *
 * Generates the next pseudo-random 32-bit value from the current state.
 * Three XOR+shift operations scramble the bits sufficiently for simple use cases.
 */
uint32_t xorshift32(void)
{
    xorshift_state ^= (xorshift_state << 13);
    xorshift_state ^= (xorshift_state >> 17);
    xorshift_state ^= (xorshift_state << 5);
    return xorshift_state;
}

ISR(PORTC_PORT_vect)
{
    if (PORTC.INTFLAGS & PIN4_bm)
    {
        if (led_state)
        {
            /* LED is ON -> turn it OFF */
            PORTE.OUTCLR = RGB_LED;
        }
        else
        {
            /* LED is OFF -> pick a random color and turn it ON */
            uint8_t color;
            PORTE.OUTCLR = RGB_LED;   /* clear any previous color first */

            do {
                color = (uint8_t)(xorshift32() & RGB_LED);
            } while (color == 0);     /* reject 0x00, would leave LED off */

            PORTE.OUTSET = color;
        }

        led_state ^= 1;             /* toggle state */
        PORTC.INTFLAGS = PIN4_bm;   /* clear interrupt flag */
    }
}

int main(void)
{
    PORTE.DIRSET = RGB_LED;

    /* External pull-down present, interrupt on rising edge (button press to Vdd) */
    PORTC.PIN4CTRL = PORT_ISC_RISING_gc;

    sei();   /* enable global interrupts and must be last step in setup */

    while (1) {}
}