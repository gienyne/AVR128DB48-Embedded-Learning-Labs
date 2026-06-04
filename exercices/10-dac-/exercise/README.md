# Exercises 10: Digital/Analog Conversion (DAC)

Work through each part in order.  
Solutions are in the `solutions/` folder. Open them only after solving each part yourself.

---

## Part 10.1: D/A Conversion with Speaker

Generate a continuous sine wave tone at 440 Hz (A4) through the speaker.

Use `sine_table` from `song.h` and a TCA0 timer interrupt to advance through  
the table at the correct rate. The DAC outputs each sample to the speaker.

**Questions to think about:**
- How do you calculate the timer period for a given frequency?
- Why is `PER + 1` used in the frequency formula?
- What does left-shifting by 6 (`<< 6`) accomplish for the DAC register?

<details>
<summary>Hint</summary>

Timer period for frequency `f` with no prescaler and `SINE_SIZE = 64`:

```c
uint16_t period = (uint16_t)(F_CPU / (F_SIGNAL * SINE_SIZE)) - 1;
```

Configure TCA0 in Normal mode, no prescaler, overflow interrupt enabled.  
In `ISR(TCA0_OVF_vect)`, write the next sine table entry to `DAC0.DATA`:

```c
DAC0.DATA = sine_table[sine_index] << 6;
sine_index = (sine_index + 1) % SINE_SIZE;
TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
```

The `<< 6` aligns the 10-bit sine value into bits 15:6 of the 16-bit DATA register.

</details>

---

## Part 10.2: Button Keyboard

Extend Part 10.1 so that pressing a button plays the corresponding note.  
Releasing all buttons stops the sound.

**Button to note mapping:**

| Pin | Note |
|-----|------|
| PA2 |   (`c`) |
| PA3 |   (`d`) |
| PA4 |   (`e`) |
| PA5 |   (`f`) |
| PA6 |   (`g`) |
| PA7 |   (`h`) |
| PB0 |   (`a`) |

Use GPIO interrupts on both edges. Start the note on press, stop it on release.

**Questions to think about:**
- How do you detect press vs release with `BOTHEDGES`?
- Why should `stop_note()` disable the timer rather than write silence to the DAC?
- Can multiple notes play simultaneously? Why or why not?

<details>
<summary>Hint</summary>

Configure all button pins with `PORT_ISC_BOTHEDGES_gc`.  
In the ISR, check `PORTA.IN & PINx_bm` to distinguish press (HIGH) from release (LOW).

```c
ISR(PORTA_PORT_vect) {
    uint8_t state = PORTA.IN;
    if      (state & PIN2_bm) start_note(c);
    else if (state & PIN3_bm) start_note(d);
    /* ... */
    else stop_note();
    PORTA.INTFLAGS = 0xFF;
}
```

`start_note()` sets `TCA0.SINGLE.PER` to the period for the requested frequency  
and enables the timer. `stop_note()` disables the timer.

</details>

---

## Part 10.3: Songs on Speaker *(Bonus)*

Play a complete song from the `song` struct automatically through the speaker.  
Implement a decay envelope so each note fades like a piano key.

**Requirements:**
- Use `mario` (or any song from `song.h`) as the melody
- Respect `bpm` and `tone_length[]` for correct timing
- Implement decay: each note starts at full volume and fades progressively
- Reset volume to maximum at the start of each new note

**Decay parameters to tune:**

| Constant | Suggested value | Effect |
|----------|----------------|--------|
| `MAX_VOLUME` | 1023 | starting volume |
| `MIN_VOLUME` | 300 | floor — note never goes fully silent |
| `DECAY_STEP` | 1 | how much volume decreases per tick |
| `DECAY_TICK_MS` | 2 | how often volume decreases (in ms) |

**Questions to think about:**
- Why must decay be applied in the 1 ms timer ISR, not in the audio ISR?
- What happens if `MIN_VOLUME = 0`? What if it is too high?
- Why use two separate timers (TCA0 and TCA1)?

<details>
<summary>Hint</summary>

Use TCA0 for audio (sine wave output) and TCA1 for timing (1 ms tick).

Note duration:
```c
uint16_t duration_ms = (60000UL / bpm) * tone_length[i];
```

In `ISR(TCA1_OVF_vect)`:
- increment `elapsed_ms`
- apply decay using a `decay_counter`
- set `note_finished = 1` when `elapsed_ms >= current_note_duration`

In `ISR(TCA0_OVF_vect)`:
```c
uint32_t sample = (sine_table[sine_index] * volume) / MAX_VOLUME;
DAC0.DATA = (uint16_t)sample;
```

In `play_song()`, reset `volume = MAX_VOLUME` before each note.

TCA1 configuration for 1 ms tick at 4 MHz / prescaler 64:
```c
TCA1.SINGLE.PER = 62;   /* (4,000,000 / 64 / 1000) - 1 ≈ 62 */
```

</details>