/*
 * main.c Part 10.3: Songs on Speaker
 *
 * Strategy:
 *   Two timers with separate responsibilities:
 *
 *   TCA0 -> audio timer (fast):
 *     Fires at the sample rate (~28,000 time per second for A4).
 *     On each overflow, writes the next sine sample scaled by current volume.
 *     Frequency is set by adjusting TCA0.SINGLE.PER.
 *
 *   TCA1 -> time reference (1 ms tick):
 *     Fires every 1 ms.
 *     Measures elapsed time to determine when to advance to the next note.
 *     Applies decay by decrementing volume every DECAY_TICK_MS milliseconds.
 *
 *   Why decay must live in TCA1 and not TCA0:
 *     TCA0 fires ~28,000 times/second for A4. Decrementing volume there
 *     would drain it from 1023 to 0 in ~36 ms. The note vanishes instantly.
 *     TCA1 fires 1000 times/second, allowing a controlled, gradual fade.
 *
 *   Decay model:
 *     volume starts at MAX_VOLUME on each new note.
 *     Every DECAY_TICK_MS milliseconds, volume decreases by DECAY_STEP.
 *     volume never goes below MIN_VOLUME, so the note never fully silences.
 *     This approximates the natural envelope of a piano key.
 *
 *   Song timing:
 *     duration_ms = (60000 / bpm) * tone_length[i]
 *     The note plays until elapsed_ms >= duration_ms (flag checked in main loop).
 *
 * Hardware:
 *   DAC output -> PD6 -> PAM8302A amplifier -> speaker
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../../lib/song.h"

#define SINE_SIZE  64

#define TCA_PER(freq) ((uint16_t)((F_CPU / ((uint32_t)(freq) * SINE_SIZE)) - 1))

#define NOTE_DURATION_MS(bpm, length) ((60000UL / (bpm)) * (length))

#define MAX_VOLUME     1023
#define MIN_VOLUME     300
#define DECAY_STEP     1
#define DECAY_TICK_MS  2

volatile uint8_t  sine_index            = 0;
volatile uint16_t volume                = MAX_VOLUME;
volatile uint8_t  decay_counter         = 0;
volatile uint16_t elapsed_ms            = 0;
volatile uint16_t current_note_duration = 0;
volatile uint8_t  note_finished         = 0;

void DAC_init(void)
{
    VREF.DAC0REF = VREF_REFSEL_2V048_gc;
    DAC0.CTRLA   = DAC_ENABLE_bm | DAC_OUTEN_bm;
    DAC0.DATA    = sine_table[0];
}

void AudioTimer_init(void)
{
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV1_gc;   /* starts disabled */
}

void TimeTimer_init(void)
{
    /* 4 MHz / 64 = 62,500 ticks/s
     * PER = (62,500 / 1000) - 1 = 62 -> ~1 ms overflow */
	
    TCA1.SINGLE.PER     = 62;
    TCA1.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA1.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV64_gc
                        | TCA_SINGLE_ENABLE_bm;
}

void set_frequency(uint16_t freq)
{
    if (freq == mute) {
        TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
    } else {
        TCA0.SINGLE.PER    = TCA_PER(freq);
        TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    }
}

ISR(TCA0_OVF_vect)
{
    /* scale sample by current volume before writing to DAC */
    uint32_t sample = ((uint32_t)sine_table[sine_index] * volume) / MAX_VOLUME;
    DAC0.DATA = (uint16_t)sample;

    sine_index++;
    if (sine_index >= SINE_SIZE) sine_index = 0;

    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(TCA1_OVF_vect)
{
    elapsed_ms++;

    /* apply decay at a controlled rate */
    decay_counter++;
    if (decay_counter >= DECAY_TICK_MS) {
        decay_counter = 0;
        if (volume > MIN_VOLUME) volume -= DECAY_STEP;
    }

    /* signal main loop when note duration has elapsed */
    if (elapsed_ms >= current_note_duration) {
        note_finished = 1;
        elapsed_ms    = 0;
    }

    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void play_song(const song* melody)
{
    for (uint16_t i = 0; i < melody->length; i++) {

        uint16_t freq = melody->tone[i];
        uint16_t duration_ms = (uint16_t)NOTE_DURATION_MS(melody->bpm, melody->tone_length[i]);

        volume = MAX_VOLUME;   // reset volume for each new note
        current_note_duration = duration_ms;
        note_finished = 0;

        set_frequency(freq);

        while (!note_finished) {}   // wait for TCA1 to signal completion 

        /* short silence between notes */
        set_frequency(mute);
        for (volatile uint32_t d = 0; d < 15000; d++) {}
    }

    set_frequency(mute);
}

int main(void)
{
    DAC_init();
    AudioTimer_init();
    TimeTimer_init();
    sei();

    play_song(&mario);

    while (1) {
		
	}
}