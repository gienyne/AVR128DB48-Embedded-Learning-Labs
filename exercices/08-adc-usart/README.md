# Exercise 08: ADC & USART

Introduction to analog signal acquisition and serial communication on the AVR128DB48.  
This exercise uses the ADC to read analog sensors and USART to transmit data to a connected computer.

> New to Microchip Studio? See the [setup guide](../../docs/microchip-studio-setup.md) first.

---

## Hardware Setup

LCD display, potentiometer, photoresistor, RGB LED, and 4 buttons.

![Versuchsaufbau 6 — LCD, potentiometer, photoresistor, RGB LED, 4 buttons](images/versuchsaufbau6.png)

| AVR128DB48 Pin | Component | Description |
|----------------|-----------|-------------|
| PF3 (AIN19) | Potentiometer (wiper) | analog voltage input |
| PF2 (AIN18) | Photoresistor (voltage divider) | analog light level input |
| PC4 – PC7 | Buttons | digital input with pull-up |
| PE0 | RGB LED — Red | PWM output |
| PE1 | RGB LED — Green | PWM output |
| PE2 | RGB LED — Blue | PWM output |
| PA2 | LCD SDA | I2C data |
| PA3 | LCD SCL | I2C clock |
| PB0 (TX) | USART3 transmit | serial output to computer |
| PB1 (RX) | USART3 receive | serial input from computer |

---

## Library Files

Parts 8.1 and 8.2 use the LCD library from Exercise 05.

```c
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"
```

---

## Concepts Used in This Exercise

<details>
<summary>ADC — Analog to Digital Conversion</summary>

The ADC converts a continuous analog voltage into a discrete digital number.  
The AVR128DB48 ADC has 12-bit resolution, producing values from 0 to 4095.

The conversion formula maps the raw ADC value to a voltage:

```
voltage (mV) = (ADC_value × V_ref × 1000) / ADC_max
             = (ADC_value × 3300) / 4095
```

Key configuration registers:

```c
VREF.ADC0REF = VREF_REFSEL_VDD_gc;          /* reference voltage = VDD (3.3V) */
ADC0.MUXPOS  = ADC_MUXPOS_AIN19_gc;         /* select input channel (PF3)     */
ADC0.CTRLA   = ADC_ENABLE_bm
             | ADC_RESSEL_12BIT_gc;          /* enable, 12-bit resolution      */
ADC0.CTRLC   = ADC_PRESC_DIV16_gc;          /* ADC clock prescaler            */
```

Starting a conversion and reading the result:

```c
ADC0.COMMAND = ADC_STCONV_bm;               /* start conversion               */
while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));   /* wait for result ready flag     */
ADC0.INTFLAGS = ADC_RESRDY_bm;             /* clear flag                     */
uint16_t result = ADC0.RES;                 /* read result                    */
```

</details>

<details>
<summary>USART: Serial Communication</summary>

USART (Universal Synchronous/Asynchronous Receiver Transmitter) enables serial  
communication between the AVR and a connected computer over two wires:

- **TX (PB0)** -> transmit: AVR sends data to computer
- **RX (PB1)** -> receive: computer sends data to AVR

**Baud rate** is the number of bits transmitted per second. Both sides must agree on  
the same rate. 9600 baud is a common standard for debugging.

The baud register value is calculated from:

```c
USART3.BAUD = (uint16_t)((F_CPU << 6) / (16L * BAUD_RATE));
```

This formula comes from the AVR128DB48 datasheet. At F_CPU = 4 MHz and 9600 baud,  
the result is approximately 1667.

Basic configuration:

```c
USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;   /* enable TX and RX           */
USART3.CTRLC = USART_CHSIZE_8BIT_gc;             /* 8 data bits per frame      */
```

Sending one character:

```c
while (!(USART3.STATUS & USART_DREIF_bm)) {}     /* wait until TX buffer empty */
USART3.TXDATAL = character;                       /* write character to TX      */
```

To view output: open **Data Visualizer** in Microchip Studio ->  
Configuration -> External Connection -> Serial Port.  
Select the Curiosity Virtual COM Port, set the baud rate, and open Terminal.

</details>

<details>
<summary>Internal Temperature Sensor and SIGROW</summary>

The AVR128DB48 has a built-in temperature sensor accessible through the ADC.  
To use it, configure the ADC with the internal 2.048V reference and select the  
temperature sense channel:

```c
VREF.ADC0REF = VREF_REFSEL_2V048_gc;
ADC0.MUXPOS  = ADC_MUXPOS_TEMPSENSE_gc;
ADC0.CTRLD   = ADC_INITDLY_DLY64_gc;   /* initialization delay >= 25 µs */
ADC0.SAMPCTRL = 28;                    /* sample time >= 28 µs           */
```

The raw ADC value is converted to Kelvin using calibration values stored in  
the device's **SIGROW** (Signature Row),factory-programmed per-chip constants:

```c
uint16_t offset = SIGROW.TEMPSENSE1;   /* offset correction  */
uint16_t slope  = SIGROW.TEMPSENSE0;   /* gain correction    */

uint32_t temp = offset - adc_value;
temp *= slope;
temp += 2048;    /* round to nearest integer */
temp /= 4096;    /* result in Kelvin         */
```

Subtract 273 to convert to Celsius.

</details>

<details>
<summary>snprintf and sscanf</summary>

`snprintf` formats data into a string, similar to `printf` but writes to a buffer  
instead of printing directly. It prevents buffer overflow by limiting output length:

```c
char buffer[64];
snprintf(buffer, sizeof(buffer), "Time: %lu s, Temp: %d C\n", seconds, temp);
```

Common format specifiers:
- `%d`  -> signed integer
- `%u`  -> unsigned integer
- `%lu` -> unsigned long
- `%s`  -> string
- `%f`  -> float (avoid on AVR - uses significant flash/RAM)

`sscanf` parses a string and extracts values into variables. The reverse of `snprintf`:

```c
uint8_t r, g, b;
sscanf(buffer, "%hhu,%hhu,%hhu", &r, &g, &b);
```

`%hhu` reads an unsigned char (8-bit). The `&` passes the address of each variable  
so `sscanf` can write the result into it.

Both functions require `#include <stdio.h>`.

> **Note:** `snprintf` and `sscanf` add significant flash and RAM usage on AVR.  
> For production embedded code, manual parsing is preferred. Here they are used  
> for clarity.

</details>

<details>
<summary>USART Receive Interrupt</summary>

For receiving data, polling would block the main loop. An interrupt is more efficient:

```c
USART3.CTRLA |= USART_RXCIE_bm;   /* enable RX complete interrupt */
```

The ISR fires each time a byte is received:

```c
ISR(USART3_RXC_vect) {
    char received = USART3_RXDATAL;   /* read received byte */
    /* process received byte */
}
```

A common pattern is to accumulate received bytes into a buffer until a terminator  
character (e.g. `.`) is detected, then set a flag for the main loop to process.

</details>

---

## Learning Goals

- Configure the ADC for analog sensor readings
- Convert raw ADC values to voltage and percentage
- Initialize and use USART for serial data transmission
- Use the internal temperature sensor with SIGROW calibration
- Receive data over USART using an interrupt-driven buffer
- Combine ADC, USART, PWM, and LCD in a single program

---

## Exercises

The exercise parts are described in [EXERCISES.md](./EXERCISES.md).  
Work through them in order. Solutions are in the `solutions/` folder — open them only after solving each part yourself.

---

## Project Structure

```
08-adc-usart/
│
├── README.md
├── EXERCISES.md
├── images/
│   └── versuchsaufbau6.png
│
├── starter/
│   ├── 8.1_potentiometer/main.c
│   ├── 8.2_photoresistor/main.c
│   ├── 8.3_buttons_over_usart/main.c
│   ├── 8.4_temperature_over_usart/main.c
│   └── 8.5_rgb_control_over_usart/main.c
│
└── solutions/
    ├── 8.1_potentiometer/main.c
    ├── 8.2_photoresistor/main.c
    ├── 8.3_buttons_over_usart/main.c
    ├── 8.4_temperature_over_usart/main.c
    └── 8.5_rgb_control_over_usart/main.c
```

---

## Resources

- [AVR128DB48 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB28-32-48-64-DataSheet-DS40002247A.pdf)
- [Microchip Studio Setup Guide](../../docs/microchip-studio-setup.md)