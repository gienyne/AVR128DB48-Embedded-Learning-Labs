/*
 * main.c Part 7.2: Gradient Rainbow LED
 *
 * Smoothly transition the RGB LED through the visible color spectrum:
 *   red -> yellow -> green -> cyan -> blue -> magenta -> red -> ...
 *
 * Hardware:
 *   RGB LED: PE0 = Red, PE1 = Green, PE2 = Blue
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

void set_rgb_brightness(uint8_t red, uint8_t green, uint8_t blue)
{
    /* YOUR CODE HERE */
}

void rainbow_transition(void)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* YOUR CODE HERE */

    while (1)
    {
        rainbow_transition();
    }
}