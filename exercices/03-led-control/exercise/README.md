# Exercises 03: LED Control

Work through each part in order.  
Solutions are in the `solutions/` folder - open them only after solving each part yourself.

---

## Part 3.1: Light Up

Set pin PD7 as an output and drive it HIGH to turn on a single LED permanently.

**Expected result:** LED on PD7 stays on.

<details>
<summary>Hint</summary>

Use `PORTD.DIRSET` to configure PD7 as an output.  
Use `PORTD.OUTSET` or `PORTD.OUT` to drive it HIGH.  
`PIN7_bm` is the bitmask for pin 7 - it is defined in `<avr/io.h>`.

</details>

---

## Part 3.2: Blinky

Make one LED (PD7) blink by toggling its state repeatedly with a delay.  
Extend to two LEDs (PD6 and PD7) blinking simultaneously.

**Expected result:** LED(s) alternate between on and off at a visible rate.

<details>
<summary>Hint</summary>

Use `PORTD.OUTTGL` to toggle a pin state in a single instruction.  
Implement the delay as a busy-wait loop using a `volatile` counter.  
the `volatile` keyword prevents the compiler from optimising the loop away.  
The constant `blink_geschwindigkeit` in `leds.h` defines the delay duration.

</details>

---

## Part 3.3: Moving Lights

Shift a single lit LED from PD0 to PD7, then back from PD7 to PD0, repeating continuously.

**Expected result:** a single lit LED sweeps back and forth across the LED bar.

<details>
<summary>Hint</summary>

Use the left shift operator `<<` to move a single bit across the byte:  
`PORTD.OUT = (PIN0_bm << i)` sets only pin `i` HIGH and clears all others in one write.  
Run two loops — one counting up from 0 to 7, one counting down from 7 to 0.  
Use a busy-wait inner loop for the delay between each step.

</details>

---

## Part 3.4: Binary Counter

Light all 8 LEDs according to a binary counting pattern.  
Count from 0 (all off) to 255 (all on) and repeat.

**Pattern:**

| PD7 | PD6 | PD5 | PD4 | PD3 | PD2 | PD1 | PD0 | Decimal |
|-----|-----|-----|-----|-----|-----|-----|-----|---------|
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
| 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 2 |
| 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 3 |
| 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 4 |
| ... | ... | ... | ... | ... | ... | ... | ... | ... |
| 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 255 |

**Expected result:** LEDs display successive binary values with a short delay between each step.

<details>
<summary>Hint</summary>

Writing a counter value directly to `PORTD.OUT` maps each bit to a pin.  
bit 0 controls PD0, bit 7 controls PD7.  
A `uint8_t` counter incremented from 0 to 255 naturally produces all 256 patterns.  
Note: a `for` loop with condition `count <= 255` on a `uint8_t` never terminates  
because `uint8_t` wraps back to 0 after 255 — use a `do/while` instead.

</details>

---

## Part 3.5: Gray Code *(Bonus)*

Modify the binary counter so that the LEDs display a Gray code sequence.  
In Gray code, only one LED changes state at each step.

**Formula:** `gray = binary ^ (binary >> 1)`

**Example:**

| Binary | Decimal | Gray Code | Decimal |
|--------|---------|-----------|---------|
| 00000000 | 0 | 00000000 | 0 |
| 00000001 | 1 | 00000001 | 1 |
| 00000010 | 2 | 00000011 | 3 |
| 00000011 | 3 | 00000010 | 2 |
| 00000100 | 4 | 00000110 | 6 |

**Expected result:** at each step, exactly one LED changes state.

<details>
<summary>Hint</summary>

Compute the Gray code value before writing to `PORTD.OUT`:  
`uint8_t gray = binaer ^ (binaer >> 1);`  
Write `gray` to the port instead of `binaer`.  
The XOR operator `^` and right shift `>>` are standard C operators available on the AVR.

</details>