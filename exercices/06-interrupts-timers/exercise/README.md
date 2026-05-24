# Exercises 06: Interrupts and Timers

Work through each part in order.  
Solutions are in the `solutions/` folder. Open them only after solving each part yourself.

---

## Part 6.1: Button Interrupt

Toggle the RGB LED using an Interrupt Service Routine triggered by button PC4.

**Behavior:**
- First press: RGB LED turns ON with a random color
- Second press: RGB LED turns OFF
- Every new activation generates a new random color

**Random color generation:** implement the Xorshift32 pseudo-random algorithm.

**Questions to think about:**
- What does `volatile` mean and why is `led_state` declared volatile?
- What is a pseudo-random number generator? What does Xorshift32 do?

<details>
<summary>Hint</summary>

Declare the ISR for `PORTC_PORT_vect`.  
Inside the ISR, check `PORTC.INTFLAGS & PIN4_bm` to confirm the interrupt came from PC4.  
Always clear the interrupt flag at the end: `PORTC.INTFLAGS = PIN4_bm`.  

Configure PC4 with pull-up and rising edge interrupt:  
`PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_RISING_gc`  

Xorshift32 algorithm:
```c
uint32_t xorshift32(uint32_t state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
```
Use `xorshift32_result & RGB_LED` to extract 3 bits for the RGB channels.  
Loop until the result is non-zero to avoid turning the LED off accidentally.

</details>

---

## Part 6.2: Timer

Write a program that displays a seconds counter on the LCD.  
The counter increments exactly once per second using the TCA0 timer interrupt.

**Expected result:** LCD shows 0, then 1, 2, 3... updating every second.

**Questions to think about:**
- How is the timer configured to overflow exactly once per second?
- Which interrupt flag must be cleared inside the ISR and how?
- Why are interrupt flags not cleared automatically?

<details>
<summary>Hint</summary>

Configure TCA0 for 1 Hz overflow at 4 MHz clock with prescaler 64:  
`TCA0.SINGLE.PER = 62500`  
`TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL0_bm | TCA_SINGLE_CLKSEL2_bm`  

Enable the overflow interrupt:  
`TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm`  

Inside `ISR(TCA0_OVF_vect)`, increment the counter, update the LCD, then clear the flag:  
`TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm`  

You need an `int_to_string()` helper to convert the counter to a string for `lcd_putString()`.

</details>

---

## Part 6.3: Traffic Light

Implement a traffic light state machine using both button interrupts and the TCA0 timer.

**States:**
- Initial state: RED (PE0)
- Button PC4 pressed: transition RED -> (yellow phase) -> GREEN
- Button PC5 pressed: transition GREEN -> (yellow phase) -> RED

**Timing:**
- The yellow phase lasts 2 seconds (timer-controlled)
- During the yellow phase, further button presses should be ignored

> **Hardware note:** the RGB LED used here has no native yellow output.  
> Blue (PE2) is used in place of yellow as a hardware limitation.  
> The state machine logic is otherwise identical to a real traffic light.

**Questions to think about:**
- Does it make sense to switch phases directly on a button press while the timer is running?
- What happens if both the button ISR and the timer ISR try to modify the state simultaneously?

<details>
<summary>Hint</summary>

Define states as constants: `ROT`, `GRUEN`, `GELB2ROT`, `GELB2GRUEN`.  
Use `PORTC_PORT_vect` for button interrupts on PC4 and PC5.  
Use `TCA0_OVF_vect` for the timer. Each overflow = 1 ms (adjust PER accordingly).  
Use a `delay` counter that decrements on each timer overflow. When it reaches 0, switch state.  

Reset the timer counter with `TCA0.SINGLE.CNT = 0` on each button press.  
Check `PORTC.INTFLAGS & PIN4_bm` and `PORTC.INTFLAGS & PIN5_bm` separately inside the ISR.

</details>

---

## Part 6.4: Programmable Timer

Extend Part 6.2 with a programmable countdown timer that lights up the RGB LED when it expires.

**Behavior:**
- The timer starts at 10 seconds and counts down
- Button PC4 adds 5 seconds to the remaining time
- Button PC5 pauses and resumes the countdown
- When the timer reaches 0: RGB LED turns red and the timer stops
- The current remaining time is always shown on the LCD

**Questions to think about:**
- How do you safely share a variable between two ISRs?
- How do you pause and resume the timer without resetting it?

<details>
<summary>Hint</summary>

Use a `volatile uint16_t timer` variable for the countdown, shared between both ISRs.  
In `ISR(PORTC_PORT_vect)`:  
- PC4 press: set a flag `pin4_interrupt = 1` (add time inside the timer ISR, not here)
- PC5 press: toggle the timer enable bit: `TCA0.SINGLE.CTRLA ^= TCA_SINGLE_ENABLE_bm`

In `ISR(TCA0_OVF_vect)`:  
- Check `pin4_interrupt` first. If set, add 5 seconds and clear the flag
- Decrement `timer` if > 0
- If `timer == 0`: light up the LED and disable the timer
- Update the LCD display
- Clear `TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm` at the end

</details>