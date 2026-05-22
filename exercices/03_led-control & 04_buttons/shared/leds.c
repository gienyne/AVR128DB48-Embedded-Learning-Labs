/*
 * leds.c
 *
 * Implementation of LED and button control functions.
 * See leds.h for pin assignments and constants.
 *
 * Target: AVR128DB48
 * Clock:  8 MHz (F_CPU)
 */

#include "leds.h"

/* -----------------------------------------------------------------------
 * Exercise 03 - LED Control
 * ----------------------------------------------------------------------- */

/*
 * blink()
 *
 * Toggles PD7 each time a volatile counter reaches blink_geschwindigkeit.
 * Must be called repeatedly from the main loop.
 *
 * The counter is declared volatile to prevent the compiler from
 * optimising away the busy-wait increment.
 */
volatile unsigned long counter = 0;

void blink(void)
{
    if (counter >= blink_geschwindigkeit) {
        PORTD.OUTTGL = PIN7_bm;
        counter = 0;
    }
    counter++;
}

/*
 * zwei_leds_blinken_lassen()
 *
 * Configures PD6 and PD7 as outputs, then toggles both simultaneously
 * each time the counter reaches blink_geschwindigkeit.
 */
void zwei_leds_blinken_lassen(void)
{
    PORTD.DIRSET = zwei_leds;

    if (counter >= blink_geschwindigkeit) {
        PORTD.OUTTGL = zwei_leds;
        counter = 0;
    }
    counter++;
}

/*
 * Lauflicht()
 *
 * Shifts a single lit LED from PD0 to PD7, then back from PD7 to PD0.
 *
 * Writing (PIN0_bm << i) to PORTD.OUT sets only pin i HIGH
 * and clears all other pins simultaneously.
 */
void Lauflicht(void)
{
    PORTD.DIRSET = alle_leds;
    PORTD.OUTCLR = alle_leds;

    for (uint8_t i = 0; i < 8; i++) {
        PORTD.OUT = (PIN0_bm << i);
        for (volatile unsigned long j = 0; j < blink_geschwindigkeit; j++) {}
    }

    for (int8_t i = 7; i >= 0; i--) {
        PORTD.OUT = (PIN0_bm << i);
        for (volatile unsigned long j = 0; j < blink_geschwindigkeit; j++) {}
    }
}

/*
 * binary_counter()
 *
 * Counts from 0 to 255 and writes each value directly to PORTD.OUT.
 * Each bit maps to one LED: bit 0 -> PD0, bit 7 -> PD7.
 */
void binary_counter(void)
{
    PORTD.DIR    = alle_leds;
    PORTD.OUTCLR = alle_leds;

    for (uint8_t count = 0; count <= 255; count++) {
        PORTD.OUT = count;
        for (volatile unsigned long i = 0; i < blink_geschwindigkeit; i++) {}
    }
}

/*
 * gray_code()
 *
 * Counts from 0 to 255 but displays each value as its Gray code equivalent.
 * Gray code ensures that only one LED changes state at each step.
 *
 * Formula: gray = binary ^ (binary >> 1)
 */
void gray_code(void)
{
    PORTD.DIR    = alle_leds;
    PORTD.OUTCLR = alle_leds;

    for (uint8_t binaer_zahl = 0; binaer_zahl <= 255; binaer_zahl++) {
        uint8_t gray_zahl = binaer_zahl ^ (binaer_zahl >> 1);
        PORTD.OUT = gray_zahl;
        for (volatile unsigned long i = 0; i < blink_geschwindigkeit; i++) {}
    }
}

/* -----------------------------------------------------------------------
 * Exercise 04 - Buttons
 * ----------------------------------------------------------------------- */

/*
 * light_by_bouton_a()
 *
 * Configures PD0 as LED output and PC4 as button input with pull-up.
 * The LED follows the button state.
 *
 * Pull-up resistor: the AVR internal pull-up keeps PC4 HIGH when open.
 * Pressing the button connects PC4 to GND (active-low).
 */
void light_by_bouton_a(void)
{
    PORTD.DIRSET   = PIN0_bm;
    PORTD.OUTCLR   = PIN0_bm;
    PORTC.DIRCLR   = PIN4_bm;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;

    while (1) {
        if (PORTC.IN & PIN4_bm) {
            PORTD.OUTSET = PIN0_bm;
        } else {
            PORTD.OUTCLR = PIN0_bm;
        }
    }
}

/*
 * light_by_bouton_b()
 *
 * All LEDs turn on when any button on PC4–PC7 is pressed.
 */
void light_by_bouton_b(void)
{
    PORTD.DIRSET = alle_leds;
    PORTD.OUTSET = alle_leds;

    PORTC.DIRCLR   = alle_taste;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

    while (1) {
        if (PORTC.IN & alle_taste) {
            PORTD.OUTSET = alle_leds;
        } else {
            PORTD.OUTCLR = alle_leds;
        }
    }
}

/*
 * Debounced_Counter()
 *
 * Increments a counter each time PC4 is pressed (debounced).
 * The counter value is displayed in binary on the LEDs.
 *
 * Debouncing: a 30 ms delay after the first falling edge filters out
 * mechanical contact bounce. A second read confirms the press is genuine.
 * The function waits for button release before accepting the next press.
 */
void Debounced_Counter(void)
{
    uint8_t zaeler = 0;

    PORTD.DIRSET   = alle_leds;
    PORTD.OUTCLR   = alle_leds;
    PORTC.DIRCLR   = PIN4_bm;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;

    while (1) {
        if (!(PORTC.IN & PIN4_bm)) {
            _delay_ms(30);

            if (!(PORTC.IN & PIN4_bm)) {
                PORTD.OUT = zaeler;
                zaeler++;

                while (!(PORTC.IN & PIN4_bm)) {}
            }
        }
    }
}

/*
 * Binary_calculator()
 *
 * A four-button binary calculator displayed on the LEDs.
 *
 * Button assignments:
 *   PC4 (plus1)  ->  increment result by 1
 *   PC5 (minus1) -> decrement result by 1
 *   PC6 (mul1)   -> left shift result by 1 (multiply by 2)
 *   PC7 (div1)   -> right shift result by 1 (divide by 2)
 *
 * Edge detection: result is updated only on a new press, not while held.
 * A 30 ms delay debounces all buttons.
 */
void Binary_calculator(void)
{
    uint8_t current_taste   = 0;
    uint8_t vorherige_taste = 0;
    uint8_t ergebnis        = 0;

    PORTD.DIRSET = alle_leds;
    PORTD.OUTCLR = alle_leds;

    PORTC.DIRCLR   = alle_taste;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

    while (1) {
        current_taste = (PORTC.IN & alle_taste);

        if (current_taste && current_taste != vorherige_taste) {
            if      (current_taste & plus1)  ergebnis++;
            else if (current_taste & minus1) ergebnis--;
            else if (current_taste & mul1)   ergebnis <<= 1;
            else if (current_taste & div1)   ergebnis >>= 1;

            PORTD.OUT = ergebnis;
        }

        vorherige_taste = current_taste;
        _delay_ms(30);
    }
}

/*
 * button_by_light()
 *
 * A reaction game: a random LED lights up and the player must press
 * the matching button to turn it off.
 *
 * LED-to-button mapping (PC4–PC7 / PD4–PD7):
 *   led_index 0 -> PIN4_bm
 *   led_index 1 -> PIN5_bm
 *   led_index 2 -> PIN6_bm
 *   led_index 3 -> PIN7_bm
 */
void button_by_light(void)
{
    PORTD.DIRSET = zufall_leds;
    PORTD.OUTCLR = zufall_leds;

    PORTC.DIRCLR   = alle_taste;
    PORTC.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN7CTRL = PORT_PULLUPEN_bm;

    while (1) {
        uint8_t led_index  = (rand() % 4);
        uint8_t led_choise = (PIN4_bm << led_index);

        PORTD.OUT = led_choise;

        while (1) {
            uint8_t gedrueckte_Button = (PORTC.IN & alle_taste);

            if (gedrueckte_Button == led_choise) {
                PORTD.OUTCLR = led_choise;
                break;
            }
            _delay_ms(30);
        }
    }
}