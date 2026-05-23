# Exercises 05: Liquid Crystal Display (LCD)

Work through each part in order.  
Solutions are in the `solutions/` folder - open them only after solving each part yourself.

---

## Part 5.1: Hello Display

Write the text `"Hello Display"` on the bottom row of the LCD.

**Expected result:**

```
                 <- row 0 (empty)
Hello Display    <- row 1
```

**Questions to think about:**
- How do you move the cursor to the second row?
- How do you write a string to the display?
- On which row does the cursor start by default after `lcd_init()`?

<details>
<summary>Hint</summary>

Use `lcd_moveCursor(x, y)` to position the cursor before writing.  
`x` is the column (0–15), `y` is the row (0 = top, 1 = bottom).  
Use `lcd_putString("Hello Display")` to write the text.  
Call `lcd_init()` once at the start of `main()` before anything else.

</details>

---

## Part 5.2: Count Up

Write a program that counts from 0 to 1000 and displays each value on the LCD.  
Use a delay of approximately 1.5 seconds between each number.

**Expected result:**

```
0       <- then 1, 2, 3 ... up to 1000
```

**Questions to think about:**
- `lcd_putString()` expects a `char*`: how do you convert an integer to a string?
- How do you clear the display before showing the next number?
- What is the maximum number of digits a 32-bit integer can have?

<details>
<summary>Hint</summary>

The LCD library has no built-in integer display function — you need to convert the number to a string first.  
Write a helper function `char* int_to_string(int32_t number, char* buffer)` that builds the string digit by digit using modulo and division by 10, then reverses it.  
Allocate a char array of size 11 (max 10 digits + null terminator) to hold the result.  
Use `lcd_clear()` between updates and `_delay_ms(1500)` for the delay.

</details>

---

## Part 5.3: Ping Pong

Write a program that moves a single `1` across both rows of the LCD, from left to right on row 0, then right to left on row 1, bouncing continuously.

**Expected result (sequence of frames):**

```
Frame 0:    1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 0)
            0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 1)

Frame 1:    0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 0)
            0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 1)

...

Frame 15:   0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1   (row 0)
            0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 1)

Frame 16:   0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 0)
            0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1   (row 1)

...

Frame 31:   0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 0)
            1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   (row 1)
```

**Constraints:** do not use any pre-built LCD movement functions.

<details>
<summary>Hint</summary>

Use a `char` array of 16 characters initialized to `'0'`.  
For each frame, set position `i` to `'1'`, write both rows using `lcd_putString()`, then reset position `i` back to `'0'`.  
For row 0, increment `i` from 0 to 15. For row 1, decrement from 15 to 0.  
Use `lcd_moveCursor(0, 0)` and `lcd_moveCursor(0, 1)` to switch rows.

</details>

---

## Part 5.4: Binary Calculator II

Extend the Binary Calculator from Exercise 04 (Part 4.3) so that the result is displayed both on the LEDs (in binary) and on the LCD (in decimal).

**Button assignments** (same as Exercise 04):

| Pin | Operation |
|-----|-----------|
| PC4 | Increment result by 1 (`+1`) |
| PC5 | Decrement result by 1 (`-1`) |
| PC6 | Left shift result by 1 (`×2`) |
| PC7 | Right shift result by 1 (`÷2`) |

**Expected result:**
- LEDs show the binary representation of the result
- LCD shows the decimal value (including negative numbers)

**Additional requirements:**
- Display negative values correctly on the LCD
- Use **bit operations** for multiplication and division (left/right shift) instead of `*` and `/`

<details>
<summary>Hint</summary>

Reuse the `int_to_string()` function from Part 5.2 extend it to handle negative numbers by checking `number < 0`, taking the absolute value, and prepending `'-'` to the result.  
Use `int32_t` for the result variable to support negative values and larger ranges.  
Call `lcd_clear()` and `lcd_moveCursor(0, 0)` before writing the updated value.  
The LED display uses `PORTD.OUT = ergebnis` as before. The lower 8 bits represent the binary value.

</details>