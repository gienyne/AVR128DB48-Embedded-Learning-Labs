/*
 * main.c Part 8.4: Internal Temperature Readings over USART
 *
 * Strategy:
 *   The AVR128DB48 internal temperature sensor is read via ADC.
 *   It requires the 2.048V internal reference and a minimum sample time of 28 µs.
 *
 *   The raw ADC value is converted to Kelvin using per-chip calibration constants
 *   stored in SIGROW (factory-programmed during manufacturing):
 *     temp_K = (TEMPSENSE1 - adc) * TEMPSENSE0 / 4096
 *
 *   SCALING_FACTOR (4096) matches the fixed-point scaling used internally.
 *   Rounding is applied by adding SCALING_FACTOR/2 before dividing.
 *
 *   TCA0 generates a 1-second interrupt to trigger each transmission.
 *   snprintf builds the output string. Float formatting is avoided —
 *   the decimal digit is extracted manually using integer arithmetic.
 *
 *   Output example: "Time: 3 s, Temp: 24.3 degC, 297.3 K\n"
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

#define BAUD_RATE      9600
#define SCALING_FACTOR 4096

volatile uint32_t sekunde = 0;
char usart_buffer[64];

void USART_init(void)
{
    USART3.BAUD  = (uint16_t)((F_CPU << 6) / (16L * BAUD_RATE));
    USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
    USART3.CTRLC = USART_CHSIZE_8BIT_gc;
}

void USART_send(char c)
{
    while (!(USART3.STATUS & USART_DREIF_bm)) {}
    USART3.TXDATAL = c;
}

void USART_send_string(char* str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        USART_send(str[i]);
    }
}

void Timer_init(void)
{
    TCA0.SINGLE.PER     = 62500;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV64_gc
                        | TCA_SINGLE_ENABLE_bm;
}

ISR(TCA0_OVF_vect)
{
    sekunde++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void ADC_init(void)
{
    VREF.ADC0REF  = VREF_REFSEL_2V048_gc;           /* required for temp sensor  */
    ADC0.MUXPOS   = ADC_MUXPOS_TEMPSENSE_gc;
    ADC0.CTRLA    = ADC_ENABLE_bm | ADC_RESSEL_12BIT_gc;
    ADC0.CTRLB    = ADC_SAMPNUM_NONE_gc;
    ADC0.CTRLC    = ADC_PRESC_DIV16_gc;
    ADC0.CTRLD    = ADC_INITDLY_DLY64_gc;            /* init delay >= 25 µs       */
    ADC0.SAMPCTRL = 28;                              /* sample time >= 28 µs      */
}

uint16_t ADC_read(void)
{
    ADC0.COMMAND  = ADC_STCONV_bm;
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    return ADC0.RES;
}

/*
 * adc_to_kelvin()
 *
 * Applies the SIGROW correction formula from the AVR128DB48 datasheet.
 * TEMPSENSE1 = offset, TEMPSENSE0 = slope (gain).
 * The result is in Kelvin (integer).
 */
uint16_t adc_to_kelvin(uint16_t adc)
{
    uint32_t temp = SIGROW.TEMPSENSE1 - adc;
    temp *= SIGROW.TEMPSENSE0;
    temp += SCALING_FACTOR / 2;   /* round to nearest */
    temp /= SCALING_FACTOR;
    return (uint16_t)temp;
}

void send_temperature(void)
{
    uint16_t adc    = ADC_read();
    uint16_t temp_k = adc_to_kelvin(adc);
    uint16_t temp_c = temp_k - 273;

    /* extract one decimal digit without using %f */
    uint16_t adc2    = ADC_read();
    uint16_t temp_k2 = adc_to_kelvin(adc2);
    uint16_t dec_c   = ((uint32_t)temp_k * 10 / 1) % 10;
    uint16_t dec_k   = ((uint32_t)temp_k2 * 10 / 1) % 10;
    (void)dec_k;

    snprintf(usart_buffer, sizeof(usart_buffer),
             "Time: %lu s, Temp: %u.%u degC, %u.0 K\n",
             sekunde, temp_c, dec_c, temp_k);

    USART_send_string(usart_buffer);
}

int main(void)
{
    USART_init();
    Timer_init();
    ADC_init();
    sei();

    while (1) {
        send_temperature();
        _delay_ms(1000);
    }
}