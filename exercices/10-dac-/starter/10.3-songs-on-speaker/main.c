/*
 * main.c Part 10.3: Songs on Speaker
 *
 * Play mario  automatically with a decay envelope.
 *
 * Requirements:
 *   - Use bpm and tone_length[] for correct timing
 *   - Decay: volume starts at MAX_VOLUME per note, fades progressively
 *   - Reset volume to MAX_VOLUME at the start of each note
 *   - Use TCA0 for audio, TCA1 for 1 ms timing
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../../lib/song.h"

#define SINE_SIZE      64
#define MAX_VOLUME     1023
#define MIN_VOLUME     300
#define DECAY_STEP     1
#define DECAY_TICK_MS  2

#define TCA_PER(freq) ((uint16_t)((F_CPU / ((uint32_t)(freq) * SINE_SIZE)) - 1))

#define NOTE_DURATION_MS(bpm, length) ((60000UL / (bpm)) * (length))

/* YOUR CODE HERE: declare shared volatile variables */

ISR(TCA0_OVF_vect)
{
    /* YOUR CODE HERE: output scaled sine sample to DAC */
}

ISR(TCA1_OVF_vect)
{
    /* YOUR CODE HERE: increment elapsed_ms, apply decay, set note_finished */
}

void play_song(const song* melody)
{
    /* YOUR CODE HERE */
}

int main(void)
{
    /* YOUR CODE HERE */
    sei();
    play_song(&mario);
    while (1) {
		
	}
}