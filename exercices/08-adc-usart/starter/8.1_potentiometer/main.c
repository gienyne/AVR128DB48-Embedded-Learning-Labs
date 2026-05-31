/*
 * main.c Part 8.1: A/D Conversion with Potentiometer
 *
 * Read the analog voltage from the potentiometer on PF3 (AIN19).
 * Display voltage in mV and position as percentage on the LCD.
 *
 * Hardware:
 *   Potentiometer wiper -> PF3 (AIN19)
 *   LCD via I2C (PA2 SDA, PA3 SCL)
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

/* YOUR CODE HERE: implement int_to_string(), ADC_init(), ADC_read() */

int main(void)
{
    /* YOUR CODE HERE */
    while (1) {
		
	}
}


int main(void)
{
    /* YOUR CODE HERE */
    sei();
    while (1) {}
}

/*
 * main.c — Part 8.4: Internal Temperature Readings over USART
 *
 * Read the internal temperature sensor and transmit readings over USART.
 * Use TCA0 for the 1-second interval. Output format:
 *   "Time: N s, Temp: XX.X degC, YYY.Y K\n"
 *
 * Hardware:
 *   USART3 TX -> PB0
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD_RATE 9600

/* YOUR CODE HERE */

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* YOUR CODE HERE */
    sei();
	
    while (1) {
	}
}