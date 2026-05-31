/*
 * main.c Part 8.1: A/D Conversion with Potentiometer
 *
 * Strategy:
 *   The potentiometer acts as a voltage divider: turning it changes the voltage
 *   on PF3 (AIN19) from 0V to VDD (3.3V). The ADC converts this voltage into
 *   a 12-bit number (0–4095).
 *
 *   Two values are derived from the raw ADC reading:
 *     voltage_mv = (adc_value * 3300) / 4095   -> voltage in millivolts
 *     percent    = (adc_value * 100)  / 4095   -> position as percentage
 *
 *   Both are displayed on the LCD and updated every 500 ms.
 *
 *   Integer arithmetic is used throughout to avoid floating-point overhead.
 *   The multiplication uses UL (unsigned long) literals to prevent overflow
 *   before the division.
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

#define ADC_MAX   4095
#define VREF_MV   3300   /* reference voltage in millivolts (VDD = 3.3V) */
#define BUF_SIZE  7

char* int_to_string(uint16_t number, char* buffer)
{
    int pos = 0;

    if (number == 0) {
        buffer[pos++] = '0';
    }

    while (number > 0) {
        buffer[pos++] = '0' + (number % 10);
        number /= 10;
    }

    buffer[pos] = '\0';

    for (int i = 0; i < pos / 2; i++) {
        char tmp          = buffer[i];
        buffer[i]         = buffer[pos - 1 - i];
        buffer[pos-1-i]   = tmp;
    }

    return buffer;
}

void ADC_init(void)
{
    VREF.ADC0REF = VREF_REFSEL_VDD_gc;             /* reference = VDD (3.3V)    */
    ADC0.MUXPOS  = ADC_MUXPOS_AIN19_gc;            /* PF3 = AIN19               */
    ADC0.CTRLA   = ADC_ENABLE_bm
                 | ADC_RESSEL_12BIT_gc;             /* enable, 12-bit resolution */
    ADC0.CTRLB   = ADC_SAMPNUM_NONE_gc;
    ADC0.CTRLC   = ADC_PRESC_DIV16_gc;
}

uint16_t ADC_read(void)
{
    ADC0.COMMAND  = ADC_STCONV_bm;
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    return ADC0.RES;
}

int main(void)
{
    char buf_v[BUF_SIZE];
    char buf_p[BUF_SIZE];

    ADC_init();
    lcd_init();
    lcd_enable(true);

    while (1) {
        uint16_t adc    = ADC_read();
        uint16_t voltage = (uint16_t)((adc * (uint32_t)VREF_MV) / ADC_MAX);
        uint16_t percent = (uint16_t)((adc * 100UL) / ADC_MAX);

        lcd_clear();
        lcd_moveCursor(0, 0);
        lcd_putString("Spannung: ");
        lcd_putString(int_to_string(voltage, buf_v));
        lcd_putString(" mV");

        lcd_moveCursor(0, 1);
        lcd_putString("Prozent: ");
        lcd_putString(int_to_string(percent, buf_p));
        lcd_putString(" %");

        _delay_ms(500);
    }
}