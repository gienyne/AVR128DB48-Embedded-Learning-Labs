# Exercise 03: LED Control

Introduction to GPIO output on the AVR128DB48.  
This exercise covers turning on, blinking, and sequencing LEDs connected to Port D.

> New to Microchip Studio? See the [setup guide](../docs/microchip-studio-setup.md) first.

---

## Hardware Setup

8 LEDs with series resistors connected to pins PD0 through PD7.  
The cathode of each LED connects to GND through a resistor.

![Versuchsaufbau — 8 LEDs with resistors](images/versuchsaufbau-leds.png)

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

## Exercises

The exercise parts are described in [EXERCISES.md](./EXERCISES.md).  
Work through them in order. Solutions are in the `solutions/` folder — open them only after solving each part yourself.

---

## Project Structure

```
03-led-control/
│
├── README.md
├── EXERCISES.md
├── images/
│   └── versuchsaufbau-leds.png
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

---

## Resources

- [AVR128DB48 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB28-32-48-64-DataSheet-DS40002247A.pdf)
- [Microchip Studio Setup Guide](../docs/microchip-studio-setup.md)