/*
 * leds.h
 *
 * Shared header for LED and button control exercises (Exercise 03 and 04).
 * Defines pin masks, timing constants, and function prototypes.
 *
 * Target: AVR128DB48
 * Clock:  8 MHz (F_CPU)
 */

#ifndef LEDS_H
#define LEDS_H

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

/*
 * Delay value used in busy-wait loops to produce a visible blink rate.
 * Increase to slow down, decrease to speed up.
 */
#define blink_geschwindigkeit 100000UL

/* -----------------------------------------------------------------------
 * LED pin masks - Port D
 * ----------------------------------------------------------------------- */

/* Two LEDs: PD6 and PD7 */
#define zwei_leds       (PIN7_bm | PIN6_bm)

/* All eight LEDs: PD0 through PD7 */
#define alle_leds       (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | \
                         PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

/* -----------------------------------------------------------------------
 * Button pin masks - Port C (PC4 through PC7)
 * ----------------------------------------------------------------------- */

/* All four buttons */
#define alle_taste      (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)

/* Individual button assignments for the binary calculator */
#define plus1           (PIN4_bm)   /* increment (+1)    */
#define minus1          (PIN5_bm)   /* decrement (-1)    */
#define mul1            (PIN6_bm)   /* left shift (*2)   */
#define div1            (PIN7_bm)   /* right shift (/2)  */

/* Four LEDs used in the button-by-light game (PC4 through PC7 range) */
#define zufall_leds     (alle_taste)

/* -----------------------------------------------------------------------
 * Function prototypes - Exercise 03: LED Control
 * ----------------------------------------------------------------------- */

/* Blink a single LED on PD7 */
void blink(void);

/* Blink two LEDs (PD6 and PD7) simultaneously */
void zwei_leds_blinken_lassen(void);

/* Shift a single lit LED back and forth across PD0–PD7 */
void Lauflicht(void);

/* Count from 0 to 255 in binary on PD0–PD7 */
void binary_counter(void);

/* Display a Gray code sequence on PD0–PD7 */
void gray_code(void);

/* -----------------------------------------------------------------------
 * Function prototypes — Exercise 04: Buttons
 * ----------------------------------------------------------------------- */

/* Turn on LED on PD0 while button on PC4 is pressed (single button) */
void light_by_bouton_a(void);

/* Turn on all LEDs while any button on PC4–PC7 is pressed */
void light_by_bouton_b(void);

/* Debounced counter: increment and display on LEDs each time PC4 is pressed */
void Debounced_Counter(void);

/* Binary calculator controlled by four buttons */
void Binary_calculator(void);

/* Light up a random LED and wait for the matching button press */
void button_by_light(void);

#endif /* LEDS_H */