/*
 * main.c Part 8.2: A/D Conversion with Photoresistor
 *
 * Strategy:
 *   The photoresistor forms a voltage divider with a fixed resistor.
 *   As light intensity increases, the photoresistor's resistance drops,
 *   raising the voltage on PF2 (AIN18).
 *
 *   Dynamic calibration: the maximum ADC value seen since startup is tracked.
 *   This value defines 100% — shining a bright light at the sensor sets the
 *   upper calibration point. Before calibration, the percentage may read 100%
 *   since adc_max equals the current reading.
 *
 *   Division-by-zero is guarded: if adc_max is still 0 on the first iteration,
 *   percentage is set to 0.
 *
 * Hardware:
 *   Photoresistor voltage divider -> PF2 (AIN18)
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
#define VREF_MV   3300
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
        char tmp        = buffer[i];
        buffer[i]       = buffer[pos - 1 - i];
        buffer[pos-1-i] = tmp;
    }

    return buffer;
}

void ADC_init(void)
{
    VREF.ADC0REF = VREF_REFSEL_VDD_gc;
    ADC0.MUXPOS  = ADC_MUXPOS_AIN18_gc;            /* PF2 = AIN18               */
    ADC0.CTRLA   = ADC_ENABLE_bm
                 | ADC_RESSEL_12BIT_gc;
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
    uint16_t adc_max = 0;

    ADC_init();
    lcd_init();
    lcd_enable(true);

    while (1) {
        uint16_t adc = ADC_read();

        /* update calibration maximum */
        if (adc > adc_max) {
            adc_max = adc;
        }

        uint16_t voltage = (uint16_t)((adc * (uint32_t)VREF_MV) / ADC_MAX);
        uint16_t percent = (adc_max > 0)
                         ? (uint16_t)((adc * 100UL) / adc_max)
                         : 0;

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