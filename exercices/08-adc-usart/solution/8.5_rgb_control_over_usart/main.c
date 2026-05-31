/*
 * main.c Part 8.5: RGB LED Control over USART
 *
 * Strategy:
 *   The program receives RGB values from a terminal over USART3.
 *   The protocol uses a '.' character as a message terminator:
 *     send "255,0,128." to set R=255, G=0, B=128.
 *
 *   The RX interrupt buffers incoming bytes one at a time.
 *   When '.' is received, the buffer is null-terminated and data_received
 *   is set, the main loop then parses and applies the RGB values.
 *
 *   sscanf with "%hhu" parses unsigned char (uint8_t) values directly.
 *   PER = 255 maps CMP values directly to 8-bit color levels (0–255).
 *
 *   The program acknowledges each command by echoing back the applied values.
 *
 * Hardware:
 *   RGB LED: PE0 (Red), PE1 (Green), PE2 (Blue)
 *   USART3:  PB0 TX, PB1 RX
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BAUD_RATE 9600

char USART_buffer[64];
volatile uint8_t usart_index  = 0;
volatile uint8_t data_received = 0;

void USART_init(void)
{
    USART3.BAUD  = (uint16_t)((F_CPU << 6) / (16L * BAUD_RATE));
    USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
    USART3.CTRLC = USART_CHSIZE_8BIT_gc;
    USART3.CTRLA |= USART_RXCIE_bm;   /* enable RX complete interrupt */
}

void USART_send(char c)
{
    while (!(USART3.STATUS & USART_DREIF_bm)) {}
    USART3.TXDATAL = c;
}

void USART_send_string(char* str)
{
    for (size_t i = 0; i < strlen(str); i++) {
        USART_send(str[i]);
    }
}

/*
 * ISR(USART3_RXC_vect)
 *
 * Fires each time a byte is received.
 * Bytes are appended to USART_buffer until '.' is detected.
 * '.' null-terminates the buffer and signals the main loop.
 */
ISR(USART3_RXC_vect)
{
    char received = USART3_RXDATAL;

    if (received == '.') {
        USART_buffer[usart_index] = '\0';
        usart_index  = 0;
        data_received = 1;
    }
    else if (usart_index < sizeof(USART_buffer) - 1) {
        USART_buffer[usart_index++] = received;
    }
    /* if buffer is full without terminator, excess bytes are silently dropped */
}

void pwm_init(void)
{
    PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;

    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc;

    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                      | TCA_SINGLE_CMP1EN_bm
                      | TCA_SINGLE_CMP2EN_bm
                      | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    TCA0.SINGLE.PER   = 255;   /* 8-bit range: CMP values map directly to 0–255 */

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc
                      | TCA_SINGLE_ENABLE_bm;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    TCA0.SINGLE.CMP0 = r;
    TCA0.SINGLE.CMP1 = g;
    TCA0.SINGLE.CMP2 = b;
}

int main(void)
{
    char reply[64];

    USART_init();
    pwm_init();

    PORTB.DIRCLR = PIN1_bm;   /* RX as input */

    sei();

    USART_send_string("RGB Control Ready. Format: r,g,b.\n");

    while (1) {
        if (data_received) {
            data_received = 0;

            uint8_t r = 0, g = 0, b = 0;
            sscanf(USART_buffer, "%hhu,%hhu,%hhu", &r, &g, &b);

            set_rgb(r, g, b);

            snprintf(reply, sizeof(reply), "Set RGB: %d, %d, %d\n", r, g, b);
            USART_send_string(reply);
        }
    }
}