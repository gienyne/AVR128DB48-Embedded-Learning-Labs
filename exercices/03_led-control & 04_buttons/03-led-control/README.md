# Exercise 03: LED Control

Introduction to GPIO output on the AVR128DB48.  
This exercise covers turning on, blinking, and sequencing LEDs connected to Port D.

> New to Microchip Studio? See the [setup guide](https://github.com/gienyne/Some-Embedded-avr128db48-projekt/blob/master/docs/microchip-studio-setup.md) first.

---

## Hardware Setup

8 LEDs with series resistors connected to pins PD0 through PD7.  
The cathode of each LED connects to GND through a resistor.

![Versuchsaufbau: 8 LEDs with resistors](images/Versuchsaufbau1.png)

| AVR128DB48 Pin | Component |
|----------------|-----------|
| PD0 – PD7 | LED 0–7 (anode) via series resistor |
| GND | Common ground rail |

---

## How to Use the Shared Files

The `../shared/` folder contains two files:

```
shared/
├── leds.h    - pin masks, constants, and function prototypes
└── leds.c    - reference implementation (solutions)
```

**Your workflow:**

1. Copy `leds.h` into your Microchip Studio project. It gives you all the `#define` constants and function prototypes you need.
2. Create your own `leds.c` alongside your `main.c` and implement the functions declared in `leds.h` yourself.
3. Include both files in your project. Your `main.c` only calls the functions - the implementation is yours to write.
4. Once you have solved a part, you can compare your implementation with the reference in `../shared/leds.c`.

> The goal is to write the function implementations yourself. Copying `leds.c` directly defeats the purpose of the exercise.

---

## Learning Goals

- Configure GPIO pins as output using `PORTD.DIRSET`
- Set, clear, and toggle pin states using `OUTSET`, `OUTCLR`, `OUTTGL`
- Implement software delays using busy-wait loops
- Use bit masks and bit shift operations to control individual or multiple pins
- Understand how binary values map directly to LED patterns

---

## Exercises

The exercise parts are described in [EXERCISES.md](https://github.com/gienyne/Some-Embedded-avr128db48-projekt/blob/master/exercices/03_led-control%20%26%2004_buttons/03-led-control/exercise/README.md).  
Work through them in order. Check `../shared/leds.c` only after solving each part yourself.

---

## Project Structure

```
03-led-control/
│
├── README.md
├── EXERCISES.md
├── images/
│   └── Versuchsaufbau1.png
│
├── starter/
│   ├── 3.1-light-up/main.c
│   ├── 3.2-blinky/main.c
│   ├── 3.3-moving-lights/main.c
│   ├── 3.4-binary-counter/main.c
│   └── 3.5-gray-code/main.c
│
└── solutions/
    ├── 3.1-light-up/main.c
    ├── 3.2-blinky/main.c
    ├── 3.3-moving-lights/main.c
    ├── 3.4-binary-counter/main.c
    └── 3.5-gray-code/main.c

shared/                        (one level up — ../shared/)
├── leds.h                     - include this in your project
└── leds.c                     - reference implementation, check after solving
```

---

## Resources

- [AVR128DB48 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB28-32-48-64-DataSheet-DS40002247A.pdf)
- [Microchip Studio Setup Guide](https://github.com/gienyne/Some-Embedded-avr128db48-projekt/blob/master/docs/microchip-studio-setup.md)
