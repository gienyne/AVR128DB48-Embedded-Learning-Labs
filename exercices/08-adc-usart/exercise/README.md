# Exercises 08: ADC & USART

Work through each part in order.  
Solutions are in the `solutions/` folder. Open them only after solving each part yourself.

---

## Part 8.1: A/D Conversion with Potentiometer

Read the analog voltage from the potentiometer connected to PF3 (AIN19).  
Display the measured voltage in millivolts and as a percentage (0–100%) on the LCD.

**Questions to think about:**
- What reference voltage is used and why does it matter?
- How do you convert a raw 12-bit ADC value to millivolts?
- What is the maximum voltage the ADC can measure with VDD as reference?

<details>
<summary>Hint</summary>

Configure the ADC with VDD as reference voltage (`VREF_REFSEL_VDD_gc`) and select  
channel AIN19 (`ADC_MUXPOS_AIN19_gc`) for PF3.

Conversion formulas:

```c
uint16_t voltage_mv = (uint16_t)((adc_value * 3300UL) / 4095);
uint16_t percent    = (uint16_t)((adc_value * 100UL)  / 4095);
```

Use `lcd_clear()` before updating and `_delay_ms(500)` between readings  
to avoid display flickering.

</details>

---

## Part 8.2: A/D Conversion with Photoresistor

Read the analog voltage from the photoresistor voltage divider connected to PF2 (AIN18).  
Display the voltage in millivolts and the light level as a percentage on the LCD.

**Calibration:** the upper bound (100%) should be set dynamically — the maximum ADC  
value seen so far becomes the reference for 100%. Point a flashlight at the sensor  
to calibrate the upper limit.

**Questions to think about:**
- Why is dynamic calibration useful for a photoresistor?
- What happens to the percentage display before any calibration has occurred?

<details>
<summary>Hint</summary>

Track the maximum ADC value seen so far:

```c
if (adc_value > adc_max) {
    adc_max = adc_value;
}
```

Use `adc_max` as the denominator for the percentage calculation:

```c
uint16_t percent = (adc_max > 0) ? (uint16_t)((adc_value * 100UL) / adc_max) : 0;
```

Guard against division by zero before `adc_max` has been set.

</details>

---

## Part 8.3: Pressed Buttons over USART

Write a program that sends a character over USART each time a button is pressed.

| Button | Character sent |
|--------|---------------|
| PC4 | `K` |
| PC5 | `A` |
| PC6 | `M` |
| PC7 | `U` |

Use interrupts on all four buttons. A single ISR handles all four pins.

**Questions to think about:**
- How can one ISR handle four different pins with only two instructions per pin?
- What baud rate should the terminal on the computer be set to?
- What is `BOTHEDGES` and why is it useful here?

<details>
<summary>Hint</summary>

Configure all four pins with `PORT_ISC_BOTHEDGES_gc` to trigger on both edges.  
Inside the ISR, check `PORTC.IN & PINx_bm` to confirm the pin is currently HIGH  
(rising edge) before counting. This filters out the falling edge automatically.

```c
PORTC.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
```

Use a volatile counter per button. In the main loop, check each counter and send  
the corresponding character if non-zero, then decrement.

Set the terminal baud rate to match `BAUD_RATE` (9600).

</details>

---

## Part 8.4: Internal Temperature Readings over USART

Use the AVR128DB48 internal temperature sensor to measure and transmit the temperature  
over USART once per second. Display both Celsius and Kelvin values.

**Output format:**
```
Time: 1 s, Temp: 24.3 degC, 297.3 K
Time: 2 s, Temp: 24.4 degC, 297.4 K
```

Use TCA0 to generate the one-second interval.

**Questions to think about:**
- Why does the internal temperature sensor require a specific reference voltage?
- What is SIGROW and why are its values different for every chip?
- Why should floating-point (`%f`) be avoided on AVR in production?

<details>
<summary>Hint</summary>

Use the internal 2.048V reference for the temperature sensor:

```c
VREF.ADC0REF  = VREF_REFSEL_2V048_gc;
ADC0.MUXPOS   = ADC_MUXPOS_TEMPSENSE_gc;
ADC0.CTRLD    = ADC_INITDLY_DLY64_gc;
ADC0.SAMPCTRL = 28;
```

Apply the SIGROW correction formula from the datasheet:

```c
uint32_t temp = SIGROW.TEMPSENSE1 - adc_value;
temp *= SIGROW.TEMPSENSE0;
temp += 2048;
temp /= 4096;   /* result in Kelvin */
```

Use `snprintf` with `%lu` for seconds and manual decimal formatting for temperature  
(extract integer part and first decimal digit separately to avoid `%f`).

</details>

---

## Part 8.5: RGB LED Control over USART

Write a program that receives RGB values from the computer over USART and sets  
the RGB LED color accordingly.

**Protocol:** send `r,g,b.` where r, g, b are values 0-255 and `.` marks the end.

**Example input:**
```
255,0,128.
```

**Expected response:**
```
Set RGB: 255, 0, 128
```

Use a receive interrupt to buffer incoming bytes until the `.` terminator is detected.

**Questions to think about:**
- Why is a terminator character needed?
- What happens if the buffer is full before a terminator arrives?
- Why is the RX interrupt a better approach than polling `USART3.STATUS`?

<details>
<summary>Hint</summary>

Enable the RX interrupt: `USART3.CTRLA |= USART_RXCIE_bm`.

In `ISR(USART3_RXC_vect)`, read `USART3_RXDATAL` and append to a buffer.  
When `.` is received, null-terminate the buffer and set `data_received = 1`.

In the main loop:

```c
if (data_received) {
    data_received = 0;
    sscanf(buffer, "%hhu,%hhu,%hhu", &r, &g, &b);
    set_rgb(r, g, b);
}
```

Configure PWM for the RGB LED as in Exercise 07 (TCA0, PORTE, Single Slope).  
Set `PER = 255` so the CMP values directly represent 8-bit color levels.

</details>