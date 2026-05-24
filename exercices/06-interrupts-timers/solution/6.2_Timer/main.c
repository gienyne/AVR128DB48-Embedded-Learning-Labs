/*
 * main.c Part 6.2: Timer - Solution
 *
 * Strategy:
 *   TCA0 is configured in Normal mode with a prescaler of 64.
 *   At F_CPU = 4 MHz, each timer tick = 1 / (4,000,000 / 64) = 16 µs.
 *   With PER = 62500, the timer overflows after 62500 * 16 µs = 1 second.
 *
 *   On each overflow, the ISR increments the seconds counter,
 *   updates the LCD, and clears the interrupt flag.
 *   The flag must be cleared manually. The hardware sets it but never clears it.
 *
 *   lcd_clear() is called before each update to remove digits from
 *   longer previous values (e.g. clearing "100" before writing "99").
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

volatile uint16_t sekunde = 0;   /* shared between ISR and main must be volatile */
char convert[6];                 /* max 5 digits for uint16_t + null terminator     */

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

    /* reverse */
    for (int i = 0; i < pos / 2; i++) {
        char tmp          = buffer[i];
        buffer[i]         = buffer[pos - 1 - i];
        buffer[pos-1-i]   = tmp;
    }

    return buffer;
}

ISR(TCA0_OVF_vect)
{
    sekunde++;
    lcd_clear();
    lcd_putString(int_to_string(convert, sekunde));

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   /* clear overflow flag */
}

int main(void)
{
    lcd_init();

    TCA0.SINGLE.PER     = 62500;                               /* overflow period = 1 s     */
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;                  /* enable overflow interrupt  */
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_ENABLE_bm                /* start timer               */
                        | TCA_SINGLE_CLKSEL0_bm               /* prescaler = 64            */
                        | TCA_SINGLE_CLKSEL2_bm;

    sei();

    while (1) {
		
	}
}