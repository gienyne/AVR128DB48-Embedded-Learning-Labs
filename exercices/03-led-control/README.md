# Exercise 03 — LED Control

Introduction to GPIO output on the AVR128DB48.  
This exercise covers turning on, blinking, and sequencing LEDs connected to Port D.

> New to Microchip Studio? See the [setup guide](../docs/microchip-studio-setup.md) first.

---

## Hardware Setup

8 LEDs with series resistors connected to pins PD0 through PD7.  
The cathode of each LED connects to GND through a resistor.

![Versuchsaufbau 1 — 8 LEDs with resistors](images/versuchsaufbau-leds.png)

**Connections:**

| AVR128DB48 Pin | Component |
|----------------|-----------|
| PD0 | LED 0 (anode) via resistor |
| PD1 | LED 1 (anode) via resistor |
| PD2 | LED 2 (anode) via resistor |
| PD3 | LED 3 (anode) via resistor |
| PD4 | LED 4 (anode) via resistor |
| PD5 | LED 5 (anode) via resistor |
| PD6 | LED 6 (anode) via resistor |
| PD7 | LED 7 (anode) via resistor |
| GND | Common ground rail |

---

## Shared Files

All parts of this exercise share the same header and source files:

```
shared/
├── leds.h    — pin masks, constants, and function prototypes
└── leds.c    — function implementations
```

Copy the `shared/` folder into your Microchip Studio project alongside your `main.c`.

---

## Learning Goals

- Configure GPIO pins as output using `PORTD.DIRSET`
- Set, clear, and toggle pin states using `OUTSET`, `OUTCLR`, `OUTTGL`
- Implement software delays using busy-wait loops
- Use bit masks and bit shift operations to control individual or multiple pins
- Understand how binary values map directly to LED patterns

---

## Parts

### Part 3.1 — Light Up

Set pin PD7 as an output and drive it HIGH to turn on a single LED.

**Expected result:** LED on PD7 stays on permanently.

<details>
<summary>Hint</summary>

Use `PORTD.DIRSET` to configure PD7 as an output.  
Use `PORTD.OUTSET` or `PORTD.OUT` to drive it HIGH.  
`PIN7_bm` is the bitmask for pin 7 — it is defined in `<avr/io.h>`.

</details>

---

### Part 3.2 — Blinky

Make one or two LEDs blink by toggling their output state repeatedly with a delay.

**Expected result:** LED(s) alternate between on and off at a visible rate.

<details>
<summary>Hint</summary>

Use `PORTD.OUTTGL` to toggle a pin state in a single instruction.  
Implement the delay as a busy-wait loop using a `volatile` counter —  
this prevents the compiler from optimising the loop away.  
The constant `blink_geschwindigkeit` in `leds.h` defines the delay duration.

</details>

---

### Part 3.3 — Moving Lights

Light up one LED at a time and shift it across all 8 LEDs from PD0 to PD7,  
then back from PD7 to PD0, repeating in a continuous loop.

**Expected result:** a single lit LED sweeps back and forth across the LED bar.

<details>
<summary>Hint</summary>

Use the left shift operator `<<` to move a single bit across the byte:  
`PORTD.OUT = (PIN0_bm << i)` sets only pin `i` HIGH.  
Run two loops — one counting up from 0 to 7, one counting down from 7 to 0.  
Use a busy-wait inner loop for the delay between each step.

</details>

---

### Part 3.4 — Binary Counter

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

Writing a counter value directly to `PORTD.OUT` maps each bit to a pin —  
bit 0 controls PD0, bit 1 controls PD1, and so on.  
A `uint8_t` counter incremented from 0 to 255 naturally produces all 256 patterns.  
Note: `uint8_t` wraps back to 0 after 255, so a `for` loop with `count <= 255`  
requires care — use `uint8_t count = 0` and check for wraparound, or use a `do/while`.

</details>

---

### Part 3.5 — Gray Code *(Bonus)*

Modify the binary counter so that the LEDs display a Gray code sequence instead  
of a standard binary count. In Gray code, only one LED changes state at each step.

**Gray code formula:** `gray = binary ^ (binary >> 1)`

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
Then write `gray` to the port instead of `binaer`.  
The XOR operator `^` and right shift `>>` are both standard C operators.

</details>

---

## Project Structure

```
03-led-control/
│
├── README.md
├── images/
│   └── versuchsaufbau_leds.png
│
├── shared/
│   ├── leds.h
│   └── leds.c
│
├── starter/
│   ├── 3.1-light-up/
│   │   └── main.c
│   ├── 3.2-blinky/
│   │   └── main.c
│   ├── 3.3-moving-lights/
│   │   └── main.c
│   ├── 3.4-binary-counter/
│   │   └── main.c
│   └── 3.5-gray-code/
│       └── main.c
│
└── solutions/
    ├── 3.1-light-up/
    │   └── main.c
    ├── 3.2-blinky/
    │   └── main.c
    ├── 3.3-moving-lights/
    │   └── main.c
    ├── 3.4-binary-counter/
    │   └── main.c
    └── 3.5-gray-code/
        └── main.c
```

> Work through `starter/` first. The solution files are commented — read them after solving.

---

## Resources

- [AVR128DB48 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB28-32-48-64-DataSheet-DS40002247A.pdf)
- [Microchip Studio Setup Guide](../docs/microchip-studio-setup.md)