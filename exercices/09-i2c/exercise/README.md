# Exercise 09: I²C

Work through each part in order.

The solution for Part 9.1 is available in the `solutions/` folder. Open it only after attempting the exercise yourself.

---

## Part 9.1: Color Sensor (TCS34725)

Use the TCS34725 color sensor to measure the color composition of incident light.

Display the RGBC (Red, Green, Blue, Clear) channel values on the LCD.

**Expected display:**

```text
R:320  G:410
B:180  C:910
```

*(Values depend on the observed object and ambient light.)*

---

### Sensor configuration to implement

| Register | Address | Value | Effect                            |
| -------- | ------- | ----- | --------------------------------- |
| ENABLE   | 0x00    | 0x03 (0000 0011)  | Power ON (PON) + ADC Enable (AEN) -> Table 5 Enable Register Datasheet|
| ATIME    | 0x01    | 0xD5 (1101 0101)  | Integration time = 103 ms -> Table 6  RGBC Timing Register Datasheet        |
| WTIME    | 0x03    | 0xFF (1111 1111)  | Wait time between measurements  -> Table 7   Wait Time Register Datasheet    |

---

### Questions to think about

* Why must `CMD_BIT (0x80)` be prepended to every register address?
* What does the value `0x94` represent?
* Why does reading require a Write transaction before the Read transaction?
* Why are 8 bytes returned by the sensor?
* How are the 8 bytes converted into 4 × 16-bit values?
* What does `ATIME = 0xD5` mean in milliseconds?

---

<details>
<summary>Hint: Sensor Initialization</summary>

Initialize the I²C peripheral before communicating with the sensor.

Each configuration command consists of:

1. Register selection
2. Register value

Example:

```c
uint8_t enable_cmd[] =
{
    CMD_BIT | ENABLE_REGISTER,
    0x03
};

i2c_write(TCS34725_ADDRESS, enable_cmd, 2);
_delay_ms(10);
```

The first byte selects the target register.

The second byte contains the value written into that register.

Add a short delay after each write operation to allow the sensor to process the configuration change.

</details>

---

<details>
<summary>Hint: Reading RGBC Values</summary>

Before reading, the starting register must be selected:

```c
uint8_t start = CMD_BIT | 0x14;   /* CDATAL */

i2c_write(TCS34725_ADDRESS, &start, 1);
_delay_ms(10);

i2c_read(TCS34725_ADDRESS, data, 8);
```

The TCS34725 automatically increments the register address during the read operation.

Therefore:

```text
0x14 -> Clear Low
0x15 -> Clear High
0x16 -> Red Low
0x17 -> Red High
0x18 -> Green Low
0x19 -> Green High
0x1A -> Blue Low
0x1B -> Blue High
```

A single read operation therefore returns all RGBC values.

</details>

---

<details>
<summary>Hint: Combining Low and High Bytes</summary>

Each channel value consists of two bytes:

```text
Low Byte
High Byte
```

Combine them into one 16-bit value:

```c
uint16_t value =
(uint16_t)(low | (high << 8));
```

Example:

```text
low  = 0xF4
high = 0x01

0x01 << 8 = 0x0100

0x0100 | 0x00F4 = 0x01F4

0x01F4 = 500
```

Result:

```text
500
```

</details>

---

<details>
<summary>Hint: LCD Display</summary>

Format each value before displaying it:

```c
char buf[16];

snprintf(
    buf,
    sizeof(buf),
    "R:%u",
    red
);

lcd_putString(buf);
```

Use `lcd_moveCursor(x, y)` to position the values on the LCD.

A 16×2 display can show two values per row.

</details>

---

## Part 9.2: I²C Traffic Analysis with a Logic Analyzer

After your program is working correctly, capture the communication between the AVR and the TCS34725 using a Logic Analyzer.

The objective is to understand how the source code translates into actual I²C bus traffic.

---

### Tasks

Capture at least:

* One register-selection transaction
* One sensor read transaction

Analyze every captured frame and answer:

1. Is it a Read or Write operation?
2. Which device address is used?
3. What data is transmitted?
4. What does the transmitted data represent?
5. Who sends the data (Master or Slave)?
6. Is there an ACK or a NACK?
7. Where is the START condition?
8. Where is the STOP condition?

---

### Required Documentation

Create a short report containing:

* Screenshots of the captured frames
* Identification of:

  * Device address
  * Read/Write bit
  * Data bytes
  * ACK/NACK
  * START condition
  * STOP condition
* A short explanation of the observed communication

> A complete example analysis is available in
> [docs/logic-analyzer-i2c.md](https://github.com/gienyne/Some-Embedded-avr128db48-projekt/blob/master/docs/logic-analyzer-i2c.pdf).
>
> Try to analyze your captures yourself before consulting it.

---

<details>
<summary>Hint: Register Selection Transaction</summary>

The following frame selects the starting register used for the RGBC readout:

```text
Setup Write to [0x29] + ACK
0x94 + ACK
```

Questions:

* Why is this a Write operation?
* What does `0x94` represent?
* Why is the Command Bit required?
* Which register is selected?

</details>

---

<details>
<summary>Hint: Sensor Read Transaction</summary>

A typical RGBC read operation looks similar to:

```text
Read [0x29] + ACK

0x34 + ACK
0x01 + ACK

0x78 + ACK
0x00 + ACK

0x6A + ACK
0x00 + ACK

0x4D + ACK
0x00 + NACK
```

Questions:

* Why does the sensor send these bytes?
* Why does the master send ACKs after each byte?
* Why is the last byte followed by a NACK?
* How can these bytes be converted into RGBC values?

</details>

---

## Learning Objectives

After completing this exercise you should be able to:

* Configure an I²C peripheral device
* Access sensor registers through I²C
* Read and display measurement values
* Interpret I²C Logic Analyzer captures
* Identify START, STOP, ACK and NACK conditions
* Distinguish between Read and Write transactions
* Relate datasheet information to source code
* Relate source code to actual bus activity
