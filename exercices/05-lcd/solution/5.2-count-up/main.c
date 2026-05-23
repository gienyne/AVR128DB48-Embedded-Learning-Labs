/*
 * main.c Part 5.2: Count Up - Solution
 *
 * Strategy:
 *   lcd_putString() expects a null-terminated char*. There is no built-in
 *   integer display function in the LCD library. We implement int_to_string()
 *   which builds the digit string using repeated modulo and division by 10,
 *   then reverses the result (digits are extracted in reverse order).
 *
 *   Buffer size: a 32-bit integer has at most 10 digits + null terminator = 11 bytes.
 *
 *   The display is cleared before each new value to avoid leftover characters
 *   from longer numbers (e.g. clearing "100" before writing "99").
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"

#define BUFFER_SIZE 11   /* max 10 digits + null terminator */

/*
 * int_to_string()
 *
 * Converts a non-negative int32_t to a decimal string.
 * Writes the result into the provided buffer and returns it.
 *
 * Algorithm:
 *   1. Handle the special case number == 0 directly.
 *   2. Extract digits using number % 10, advance buffer position.
 *   3. Reduce number using number /= 10 until 0.
 *   4. Digits are now in reverse order — reverse the string in place.
 */
char* int_to_string(int32_t number, char* buffer)
{
    int position = 0;

    if (number == 0) {
        buffer[position++] = '0';
    }

    while (number > 0) {
        buffer[position++] = '0' + (number % 10);
        number /= 10;
    }

    buffer[position] = '\0';

    /* reverse the string */
    for (int i = 0; i < position / 2; i++) {
        char tmp              = buffer[i];
        buffer[i]             = buffer[position - 1 - i];
        buffer[position-1-i]  = tmp;
    }

    return buffer;
}

void count_up(void)
{
    char buffer[BUFFER_SIZE];

    for (uint32_t i = 0; i <= 1000; i++) {
        lcd_putString(int_to_string(i, buffer));
        _delay_ms(1500);
        lcd_clear();
    }
}

int main(void)
{
    lcd_init();

    while (1)
    {
        count_up();
    }
}