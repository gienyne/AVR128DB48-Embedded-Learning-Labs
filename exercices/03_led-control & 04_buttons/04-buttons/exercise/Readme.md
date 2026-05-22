# Exercises 04: Button Integration

Work through each part in order.  
Solutions are in the `solutions/` folder - open them only after solving each part yourself.

---

## Part 4.1: Light By Button

Configure one LED (PD0) and one button (PC4).  
The LED turns on while the button is held, and turns off when released.  
Extend to all LEDs (PD0–PD7) turning on when any button (PC4–PC7) is pressed.

**Expected result:** LED follows the button state in real time.

<details>
<summary>Hint</summary>

Use `PORTC.DIRCLR = PIN4_bm` to configure PC4 as an input.  
Use `PORTC.PIN4CTRL = PORT_PULLUPEN_bm` to enable the internal pull-up resistor.  
The buttons are **active-low**: the pin reads HIGH when open, LOW when pressed.  
Read the pin state with `PORTC.IN & PIN4_bm` inside a `while(1)` loop.

</details>

---

## Part 4.2: Debounced Counter

Each time button PC4 is pressed, increment a counter and display its value in binary on the LEDs.

**Expected result:** LEDs show the binary value of the press count.  
Pressing once increments by exactly 1, even if the button bounces.

<details>
<summary>Hint</summary>

Mechanical buttons produce multiple rapid transitions (bounce) on each press.  
To debounce: detect the first falling edge, wait 30 ms, then confirm the pin is still LOW.  
After incrementing, wait in a loop until the button is released before accepting the next press.  
This prevents one physical press from being counted multiple times.

</details>

---

## Part 4.3: Binary Calculator

Implement a 4-button binary calculator. Each button performs one operation on a running result, displayed in binary on the LEDs.

**Button assignments:**

| Pin | Operation |
|-----|-----------|
| PC4 | Increment result by 1 (`+1`) |
| PC5 | Decrement result by 1 (`-1`) |
| PC6 | Left shift result by 1 (`×2`) |
| PC7 | Right shift result by 1 (`÷2`) |

**Expected result:** LEDs display the updated result after each button press.

<details>
<summary>Hint</summary>

Read all four buttons at once with `PORTC.IN & alle_taste`.  
Use edge detection: store the previous button state and act only when the  
current state differs — this prevents repeated operations while a button is held.  
A 30 ms delay at the end of each loop iteration debounces all buttons.

</details>

---

## Part 4.4: Button by Light *(Bonus)*

A reaction game: a random LED lights up on PD4–PD7.  
The player must press the matching button (PC4–PC7) to turn it off.  
A new random LED then lights up immediately.

**Expected result:** the game runs continuously. Press the correct button to advance.

<details>
<summary>Hint</summary>

Use `rand() % 4` to pick a random index between 0 and 3.  
Shift `PIN4_bm` left by that index to select the corresponding pin:  
`led_choise = (PIN4_bm << led_index)`.  
The LED and its matching button share the same pin number (PC4 matches PD4, etc.).  
Wait in an inner loop until `PORTC.IN & alle_taste` equals `led_choise`.

</details>