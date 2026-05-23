/*
 * main.c Part 5.4: Binary Calculator II - Solution
 *
 * Strategy:
 *   Extends the Binary Calculator from Exercise 04 (Part 4.3) with LCD output.
 *   The result is displayed in binary on the LEDs and in decimal on the LCD.
 *
 *   int_to_string() is extended to handle negative numbers:
 *     - detect number < 0, take absolute value, prepend '-' at the end before reversing.
 *
 *   int32_t is used instead of uint8_t to support negative values and a wider range.
 *   The lower 8 bits of ergebnis are written to PORTD.OUT for the LED display.
 *
 *   Edge detection (current vs previous button state) prevents repeated updates
 *   while a button is held. A 30 ms delay debounces all four buttons.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"
#include "../../shared/leds.h"

#define BUFFER_SIZE 12   /* sign + max 10 digits + null terminator */

/*
 * int_to_string()
 *
 * Converts an int32_t (including negative values) to a decimal string.
 */
char* int_to_string(int32_t number, char* buffer)
{
    int position  = 0;
    int isnegativ = 0;

    if (number < 0) {
        isnegativ = 1;
        number    = -number;   /* work with absolute value */
    }

    if (number == 0) {
        buffer[position++] = '0';
    }

    while (number > 0) {
        buffer[position++] = '0' + (number % 10);
        number /= 10;
    }

    if (isnegativ) {
        buffer[position++] = '-';   /* will be reversed to front */
    }

    buffer[position] = '\0';

    /* reverse the string */
    for (int i = 0; i < position / 2; i++) {
        char tmp             = buffer[i];
        buffer[i]            = buffer[position - 1 - i];
        buffer[position-1-i] = tmp;
    }

    return buffer;
}

void Binary_calculator(void)
{
    uint8_t current_taste   = 0;
    uint8_t vorherige_taste = 0;
    int32_t ergebnis        = 0;
    char    buffer[BUFFER_SIZE];

    PORTD.DIRSET = alle_leds;
    PORTD.OUTCLR = alle_leds;

    PORTC.DIRCLR   = alle_taste;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

    while (1) {
        current_taste = (PORTC.IN & alle_taste);

        if (current_taste && current_taste != vorherige_taste) {

            if      (current_taste & plus1)  ergebnis++;
            else if (current_taste & minus1) ergebnis--;
            else if (current_taste & mul1)   ergebnis <<= 1;   /* multiply by 2 using bit shift */
            else if (current_taste & div1)   ergebnis >>= 1;   /* divide by 2 using bit shift   */

            PORTD.OUT = (uint8_t)ergebnis;   /* lower 8 bits -> LED binary display */

            lcd_clear();
            lcd_moveCursor(0, 0);
            lcd_putString(int_to_string(ergebnis, buffer));   /* decimal display on LCD */
        }

        vorherige_taste = current_taste;
        _delay_ms(30);
    }
}

int main(void)
{
    lcd_init();

    while (1)
    {
        Binary_calculator();
    }
}