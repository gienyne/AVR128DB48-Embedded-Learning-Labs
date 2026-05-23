/*
 * main.c Part 5.1: Hello Display - Solution
 *
 * Strategy:
 *   Call lcd_init() to initialize the display and I2C bus.
 *   Use lcd_moveCursor(0, 1) to position the cursor on the second row.
 *   Use lcd_putString() to write the text.
 *
 *   lcd_moveCursor(x, y): x = column (0-15), y = row (0 = top, 1 = bottom).
 *   The display starts with the cursor on row 0 after init.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"

void print_hello_display(void)
{
    lcd_moveCursor(0, 1);          /* move cursor to column 0, row 1 (bottom) */
    lcd_putString("Hello Display"); /* write string at current cursor position */
}

int main(void)
{
    lcd_init();

    while (1)
    {
        print_hello_display();
    }
}