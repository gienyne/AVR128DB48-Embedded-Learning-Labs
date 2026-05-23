/*
 * main.c - Part 5.3: Ping Pong 
 *
 * Strategy:
 *   Use a char array of 16 characters initialized to '0'.
 *   For each frame, set one position to '1', write both rows to the LCD,
 *   then reset that position back to '0' before moving to the next frame.
 *
 *   Phase 1 (i = 0 to 15):
 *      The '1' moves from left to right on row 0.
 *
 *   Phase 2 (i = 15 to 0):
 *      The '1' moves from right to left on row 1.
 *
 *   lcd_moveCursor(0, y) repositions the cursor to the start
 *   of the selected row before writing the full 16-character string.
 *
 * Target: AVR128DB48
 */

#include <avr/io.h>
#include <util/delay.h>
#include "../../lib/AVR128DB48_I2C.h"
#include "../../lib/I2C_LCD.h"

/* Constant empty LCD row */
const char empty_row[] = "0000000000000000";

/**
 * @brief Displays a simple ping pong animation on a 16x2 LCD.
 */
void ping_pong(void)
{
    /* 16 visible characters + null terminator */
    char row[17];

    /* Initialize all positions with '0' */
    for (int i = 0; i < 16; i++) {
        row[i] = '0';
    }

    /* End of string character */
    row[16] = '\0';

    /*'1' moves from left to right on the first LCD row */
    for (int i = 0; i < 16; i++) {

        /* Activate current position */
        row[i] = '1';

        /* Display animated row on line 0 */
        lcd_moveCursor(0, 0);
        lcd_putString(row);

        /* Keep second row empty */
        lcd_moveCursor(0, 1);
        lcd_putString(empty_row);

        /* Reset current position for next iteration */
        row[i] = '0';

        /* Animation speed */
        _delay_ms(500);
    }

    /* '1' moves from right to left on the second LCD row */
    for (int end = 15; end >= 0; end--) {

        /* Activate current position */
        row[end] = '1';

        /* Keep first row empty */
        lcd_moveCursor(0, 0);
        lcd_putString(empty_row);

        /* Display animated row on line 1 */
        lcd_moveCursor(0, 1);
        lcd_putString(row);

        /* Reset current position for next iteration */
        row[end] = '0';

        /* Animation speed */
        _delay_ms(500);
    }
}

int main(void)
{
    /* Initialize LCD display */
    lcd_init();

    while (1)
    {
        /* Run ping pong animation continuously */
        ping_pong();
    }
}