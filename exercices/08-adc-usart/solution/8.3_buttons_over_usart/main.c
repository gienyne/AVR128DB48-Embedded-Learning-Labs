/*
 * main.c Part 8.3: Pressed Buttons over USART
 *
 * Strategy:
 *   All four buttons share one ISR (PORTC_PORT_vect).
 *   Each pin is configured with BOTHEDGES. The ISR fires on both rising and
 *   falling edges. Inside the ISR, PORTC.IN is checked to distinguish them:
 *   only a HIGH reading (rising edge) increments the counter.
 *   This avoids configuring four separate interrupt vectors.
 *
 *   The main loop drains each counter and sends the corresponding character.
 *   Sending from the main loop rather than inside the ISR keeps the ISR short
 *   and avoids blocking inside an interrupt context.
 *
 * Hardware:
 *   Buttons -> PC4 (K), PC5 (A), PC6 (M), PC7 (U)
 *   USART3  -> PB0 TX
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_RATE 9600

volatile uint8_t counter_4 = 0;
volatile uint8_t counter_5 = 0;
volatile uint8_t counter_6 = 0;
volatile uint8_t counter_7 = 0;

void USART_init(void)
{
    USART3.BAUD  = (uint16_t)((F_CPU << 6) / (16L * BAUD_RATE));
    USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
    USART3.CTRLC = USART_CHSIZE_8BIT_gc;
}

void USART_send(char c)
{
    while (!(USART3.STATUS & USART_DREIF_bm)) {
		
	}
    USART3.TXDATAL = c;
}

ISR(PORTC_PORT_vect)
{
    if (PORTC.INTFLAGS & PIN4_bm) {
        if (PORTC.IN & PIN4_bm) counter_4++;
        PORTC.INTFLAGS = PIN4_bm;
    }
    if (PORTC.INTFLAGS & PIN5_bm) {
        if (PORTC.IN & PIN5_bm) counter_5++;
        PORTC.INTFLAGS = PIN5_bm;
    }
    if (PORTC.INTFLAGS & PIN6_bm) {
        if (PORTC.IN & PIN6_bm) counter_6++;
        PORTC.INTFLAGS = PIN6_bm;
    }
    if (PORTC.INTFLAGS & PIN7_bm) {
        if (PORTC.IN & PIN7_bm) counter_7++;
        PORTC.INTFLAGS = PIN7_bm;
    }
}

int main(void)
{
    PORTC.DIRCLR   = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;

    USART_init();
    sei();

    while (1) {
        if (counter_4 > 0) { USART_send('K'); counter_4--; }
        if (counter_5 > 0) { USART_send('A'); counter_5--; }
        if (counter_6 > 0) { USART_send('M'); counter_6--; }
        if (counter_7 > 0) { USART_send('U'); counter_7--; }
    }
}