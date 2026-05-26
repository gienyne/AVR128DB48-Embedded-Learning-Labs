# Exercises 07: Pulse Width Modulation (PWM)

Work through each part in order.  
Solutions are in the `solutions/` folder. Open them only after solving each part yourself.

---

## Part 7.1: Dimming Red LED

Write a program that lights the red LED on PD0 at exactly 25% brightness using hardware PWM.

Implement a helper function:

```c
void set_brightness(uint8_t percent);
```

that accepts a value from 0 to 100 and sets the PWM duty cycle accordingly.  
Define the brightness level using a `#define` so it can be changed easily.

**Questions to think about:**
- What does `PORTMUX.TCAROUTEA` do and why is it needed?
- What is the relationship between `PER` and `CMP0`?
- What register enables the PWM output on the physical pin?

<details>
<summary>Hint</summary>

Configure PD0 as output with `PORTD.DIRSET = PIN0_bm`.  
Route TCA0 to Port D: `PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc`.  
Enable CMP0 output and select Single Slope mode in `CTRLB`:

```c
TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                  | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
```

Set `PER = 100` and compute the compare value:

```c
TCA0.SINGLE.CMP0 = (percent * TCA0.SINGLE.PER) / 100;
```

Start the timer in `CTRLA` with a prescaler and `TCA_SINGLE_ENABLE_bm`.

</details>

---

## Part 7.2: Gradient Rainbow LED

Write a program that smoothly transitions the RGB LED through the full visible color spectrum:

```
red -> yellow -> green -> .... -> blue -> magenta -> ... --> red -> ...
```
![Abbildung 1: Verlauf des sichtbaren Lichtspektrums](./images/Abbildung1.png)
Find an appropriate step size and delay so the transition is visibly smooth.

**Questions to think about:**
- How do you enable three PWM channels simultaneously?
- What step size and delay produce a smooth visible transition?
- Why does the spectrum run in that specific order?

<details>
<summary>Hint</summary>

Enable all three compare channels in `CTRLB`:

```c
TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm
                  | TCA_SINGLE_CMP1EN_bm
                  | TCA_SINGLE_CMP2EN_bm
                  | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
```

Route TCA0 to Port E: `PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTE_gc`.  
CMP0 = red (PE0), CMP1 = green (PE1), CMP2 = blue (PE2).

Each color transition increments one channel from 0 to 100 while keeping the others fixed.  
Example: red to yellow: red stays at 100, green increases from 0 to 100, blue stays at 0.  
A step size of 1 with a delay of 10–30 ms per step produces a smooth result.

</details>

---

## Part 7.3: Waving Servomotor

Write a program that moves the servo motor through its full range of motion continuously.

**Phase 1:** move to center, then full left, then full right (one position per second).

**Phase 2:** then sweep continuously and smoothly from left to right and back, in a loop.

**Pulse width reference:**

| Position | Pulse width | Timer ticks (DIV8, 4 MHz) |
|----------|-------------|---------------------------|
| Full left | 1.0 ms | 500 |
| Center | 1.5 ms | 750 |
| Full right | 2.0 ms | 1000 |

![Abbildung 2:  PWM-Signal zur Ansteuerung des Servomotors](./images/Abbildung2.png)

**Questions to think about:**
- Why does the servo need a 20 ms period specifically?
- Why is a small incremental step with a short delay better than jumping directly between positions?
- What happens if the delay between steps is very small? Very large?

<details>
<summary>Hint</summary>

The servo requires a 50 Hz signal (period = 20 ms).  
With F_CPU = 4 MHz and prescaler DIV8:

```
500,000 ticks/s × 0.020 s = 10,000 ticks per period
```

Set `PER = 10000`.  
Route TCA0 to Port F: `PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTF_gc`.  
Configure PF4 as output and enable CMP0 on that channel.

For smooth continuous movement, increment `CMP0` by a small value (e.g. 10) each step  
and add a short delay (e.g. `_delay_ms(10)`) between steps.  
Run one loop from left to right (500 -> 1000), then reverse (1000 -> 500).

</details>
