![AVR128DB48](https://img.shields.io/badge/MCU-AVR128DB48-red)
![Language](https://img.shields.io/badge/Language-C-blue)
![IDE](https://img.shields.io/badge/IDE-Microchip%20Studio-lightgrey)
![I2C](https://img.shields.io/badge/I2C-Protocol-orange)
![USART](https://img.shields.io/badge/USART-Communication-purple)
![PWM](https://img.shields.io/badge/PWM-Timer-green)
![ADC](https://img.shields.io/badge/ADC-Analog-yellow)
![DAC](https://img.shields.io/badge/DAC-Audio-blue)

# Learn Embedded Systems with AVR128DB48

A practical embedded systems learning project built around the **AVR128DB48** microcontroller.
Work through progressively challenging exercises, starting with software simulation and
advancing to real hardware programming using sensors, actuators, displays, and communication interfaces.

Each exercise includes:
- A clear problem statement with learning goals
- Concept blocks explaining the theory behind what you implement
- Guided hints without giving away the solution
- Annotated starter code
- A fully commented reference solution

---

## How to Use This Repo

1. Open the exercise folder you want to work on
2. Read `README.md`: hardware setup, concepts, learning goals
3. Read `EXERCISES.md`: the parts to implement, with hints
4. Write your solution in `starter/` before looking at anything else
5. Check `solutions/` only after you have a working attempt
6. Read the comments in the solution. They explain the *why*, not just the *what*

---

## Exercises

| # | Topic | Tool | Hardware |
|---|-------|------|----------|
| [01](./exercices/01_assembler_simulator/) | Assembler Simulator | Browser | None |
| [02](./exercices/02-system-programming-c/) | Low-Level Programming in C | Microchip Studio | AVR128DB48 |
| [03](https://github.com/gienyne/AVR128DB48-Embedded-Learning-Labs/tree/master/exercices/03_led-control%20%26%2004_buttons/03-led-control) | LED Control | Microchip Studio | AVR128DB48 |
| [04](https://github.com/gienyne/AVR128DB48-Embedded-Learning-Labs/tree/master/exercices/03_led-control%20%26%2004_buttons/04-buttons) | Button Integration | Microchip Studio | AVR128DB48 |
| [05](./exercices/05-lcd/) | Liquid Crystal Display | Microchip Studio | AVR128DB48 |
| [06](./exercices/06-interrupts-timers/) | Interrupts and Timers | Microchip Studio | AVR128DB48 |
| [07](./exercices/07-pwm/) | Pulse Width Modulation | Microchip Studio | AVR128DB48 |
| [08](./exercices/08-adc-usart/) | ADC & USART | Microchip Studio | AVR128DB48 |
| [09](./exercices/09-i2c/) | I²C: Color Sensor | Microchip Studio | AVR128DB48 |
| [10](./exercices/10-dac/) | Digital/Analog Conversion *(Bonus)* | Microchip Studio | AVR128DB48 |
| [11](./exercices/11-signal-processing/) | Signal Processing - IR Remote | Microchip Studio | AVR128DB48 |

---

## What You Will Build

### Exercise 01: Assembler Simulator
Write your first Assembly programs in a browser-based 8-bit simulator.  
No hardware required. Learn registers, memory, loops, arithmetic, and basic algorithms.

### Exercise 02: Low-Level Programming in C
Introduction to embedded C on the AVR128DB48 using Microchip Studio.  
Write header files, define custom types, and implement sensor interface prototypes.

### Exercise 03: LED Control
Control 8 LEDs on Port D using GPIO registers.  
Implement blinking, a moving light sweep, and a binary counter.  
Learn `DIRSET`, `OUTSET`, `OUTCLR`, `OUTTGL`, bitmasks, and busy-wait delays.

### Exercise 04: Button Integration
Read 4 buttons on Port C using pull-up resistors and active-low logic.  
Implement debouncing, edge detection, a press counter, and a 4-button binary calculator.

### Exercise 05: Liquid Crystal Display
Drive a 16*2 LCD over I²C using the provided library.  
Display text, numbers, and animated patterns. Implement integer-to-string conversion.  
Extend the binary calculator to show results on the LCD.

### Exercise 06: Interrupts and Timers
Write Interrupt Service Routines for GPIO events and the TCA0 hardware timer.  
Build a seconds counter, a traffic light state machine, and a programmable countdown timer.  
Learn `volatile`, interrupt flags, and the difference between polling and interrupts.

### Exercise 07: Pulse Width Modulation
Generate hardware PWM using TCA0 in Single Slope mode.  
Dim a red LED, produce a full rainbow color sweep on an RGB LED,  
and control a servo motor position with precise pulse widths.  
Learn PORTMUX, compare channels, and the difference between LED PWM and servo PWM.

### Exercise 08: ADC & USART
Read analog sensors (potentiometer, photoresistor, internal temperature sensor) using the ADC.  
Transmit data to a computer over USART and receive RGB color commands from a terminal.  
Learn baud rate configuration, serial framing, interrupt-driven receive buffering,  
`snprintf`, `sscanf`, and the SIGROW temperature calibration constants.

### Exercise 09: I²C Color Sensor
Communicate with the TCS34725 color sensor over I²C.  
Configure sensor registers, perform a combined Write-then-Read transaction,  
and reconstruct 16-bit channel values from pairs of 8-bit registers.  
Includes a Logic Analyzer analysis document in `docs/`.

### Exercise 10: Digital/Analog Conversion *(Bonus)*
Generate audio signals using the DAC and a sine wave lookup table.  
Build a 7-button musical keyboard and a song player with a piano-style decay envelope.  
Learn DAC configuration, sine wave generation via timer interrupt, and two-timer architecture.

### Exercise 11: Signal Processing
Decode NEC protocol IR remote control signals using edge-triggered interrupts  
and pulse width measurement. Build a fully IR-controlled countdown timer  
displayed on the LCD. No reference solution. Design your own implementation  
using everything from the previous exercises.

---

## Hardware Requirements

All exercises from 02 onwards use the **AVR128DB48 Curiosity Nano** development board.

| Component | Used in | Notes |
|-----------|---------|-------|
| AVR128DB48 Curiosity Nano | 02–11 | Main microcontroller board |
| 8 * red LED | 03, 04, 08 | with series resistors (~470Ω) |
| 4 * push button | 04, 06, 08 | with external pull-up resistors |
| HD44780 1602 LCD + HW-061 I²C module | 05, 06, 08, 09, 11 | I²C address 0x27 |
| RGB LED (common cathode) | 06, 07, 08 | with series resistors |
| Potentiometer (10kΩ) | 08 | voltage divider on PF3 |
| Photoresistor | 08 | voltage divider on PF2 |
| Servo motor | 07 | signal on PF4, powered from 5V rail |
| TCS34725 color sensor | 09 | I²C address 0x29 |
| PAM8302A amplifier + passive speaker | 10 | DAC output on PD6 |
| 7 * push button (keyboard) | 10 | on PA2–PA7, PB0 |
| IR receiver module (38 kHz) | 11 | signal on PD0 |
| Breadboard + jumper wires | all | |
| USB cable | all | for programming and power |

> **Note:** exact pin assignments, resistor values, and wiring details are described  
> in the `README.md` of each individual exercise.

---

## Tools

| Exercise | Tool |
|----------|------|
| 01 | [Simple 8-bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/): browser, no install |
| 02–11 | [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio) |

See the [Microchip Studio setup guide](./docs/microchip-studio-setup.md) before starting Exercise 02.

---

## Repository Structure

```
./
│
├── README.md                          <- this file
│
├── docs/
│   ├── microchip-studio-setup.md     <- IDE setup guide for Exercises 02–11
│   └── logic-analyzer-i2c.md         <- I²C bus analysis with Logic Analyzer (Exercise 09)
│
└── exercices/
    ├── shared/
    │   ├── leds.h                     <- shared header for Exercises 03 and 04
    │   └── leds.c                     <- shared implementation (reference)
    │
    ├── 01_assembler_simulator/
    ├── 02-system-programming-c/
    ├── 03-led-control/
    ├── 04-buttons/
    ├── 05-lcd/
    │   └── lib/                       <- I²C and LCD libraries + song.h (David Lotz / THM)
    ├── 06-interrupts-timers/
    ├── 07-pwm/
    ├── 08-adc-usart/
    ├── 09-i2c/
    │   └── docs/                      <- Logic Analyzer analysis
    ├── 10-dac/
    └── 11-signal-processing/
```

---

*Exercise topics and hardware setup are based on course materials by Jakob Czekansky*  
*Technische Hochschule Mittelhessen. All implementations, explanations, and annotations are original work.*

---

## Author

**Dimitry Ntofeu Nyatcha**  

Email: ntofeunyatchadimitry@gmail.com

Suggestions, feedback, and collaboration ideas are welcome.
