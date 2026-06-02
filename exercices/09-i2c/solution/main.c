/*
 * main.c Part 9.1: Color Sensor (TCS34725)
 *
 * Strategy:
 *   The TCS34725 is configured over I²C, then RGBC values are read
 *   continuously and displayed on the LCD.
 *
 *   Initialization sequence:
 *     1. ENABLE register (0x00): write 0x03 to set PON (Power ON) and AEN (ADC Enable)
 *     2. ATIME register  (0x01): write 0xD5 for ~103 ms integration time
 *     3. WTIME register  (0x03): write 0xFF for maximum wait time between measurements
 *
 *   Every register access must prepend CMD_BIT (0x80) to the register address.
 *   This is required by the TCS34725 protocol (bit 7 of the command byte must be 1).
 *
 *   Reading RGBC values:
 *     1. Write the starting register address (CMD_BIT | 0x14 = 0x94) to tell the
 *        sensor where to begin reading. 0x14 is CDATAL, the first register of the
 *        RGBC data block.
 *     2. Read 8 bytes. The sensor auto-increments through registers 0x14–0x1B.
 *     3. Combine each low/high byte pair into a uint16_t:
 *        value = (uint16_t)(low | (high << 8))
 *
 *   Register layout (8 bytes):
 *     data[0,1] = Clear (CDATAL, CDATAH)
 *     data[2,3] = Red   (RDATAL, RDATAH)
 *     data[4,5] = Green (GDATAL, GDATAH)
 *     data[6,7] = Blue  (BDATAL, BDATAH)
 *
 * Hardware:
 *   TCS34725 SDA -> PA2, SCL -> PA3 (shared I²C bus with LCD)
 *   LCD via I²C (address 0x27)
 *
 * Target: AVR128DB48
 */

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../../05-lcd/lib/AVR128DB48_I2C.h"
#include "../../05-lcd/lib/I2C_LCD.h"

/* TCS34725 I²C address and register addresses */
#define TCS34725_ADDRESS  0x29
#define ENABLE_REGISTER   0x00
#define ATIME_REGISTER    0x01
#define WTIME_REGISTER    0x03
#define CMD_BIT           0x80   /* must be set in bit 7 of every command byte */

uint16_t red, green, blue, clear;
uint8_t  data[8];

/*
 * sensor_init()
 *
 * Sends the initialization sequence to the TCS34725 over I²C.
 * Each command is a 2-byte array: [CMD | register_address, value].
 * A 10 ms delay is added after each write to allow the sensor to process it.
 */
void sensor_init(void)
{
    uint8_t enable_cmd[] = { CMD_BIT | ENABLE_REGISTER, 0x03 };
    i2c_write(TCS34725_ADDRESS, enable_cmd, 2);
    _delay_ms(10);

    uint8_t atime_cmd[]  = { CMD_BIT | ATIME_REGISTER,  0xD5 };
    i2c_write(TCS34725_ADDRESS, atime_cmd, 2);
    _delay_ms(10);

    uint8_t wtime_cmd[]  = { CMD_BIT | WTIME_REGISTER,  0xFF };
    i2c_write(TCS34725_ADDRESS, wtime_cmd, 2);
    _delay_ms(10);
}

/*
 * merge_bytes()
 *
 * Combines a low byte and a high byte into a 16-bit value.
 * The high byte is shifted left by 8 bits, then ORed with the low byte.
 *
 * Example: low = 0xF4, high = 0x01
 *   high << 8 = 0x0100
 *   OR low    = 0x01F4 = 500
 */
uint16_t merge_bytes(uint8_t low, uint8_t high)
{
    return (uint16_t)(low | (high << 8));
}

/*
 * read_rgbc()
 *
 * Selects register 0x14 (CDATAL) as the starting address,
 * then reads 8 bytes of RGBC data in a single I²C read transaction.
 * The sensor auto-increments through registers 0x14 to 0x1B.
 */
void read_rgbc(void)
{
    uint8_t start = CMD_BIT | 0x14;
    i2c_write(TCS34725_ADDRESS, &start, 1);
    _delay_ms(10);

    i2c_read(TCS34725_ADDRESS, data, 8);
    _delay_ms(10);

    clear = merge_bytes(data[0], data[1]);
    red   = merge_bytes(data[2], data[3]);
    green = merge_bytes(data[4], data[5]);
    blue  = merge_bytes(data[6], data[7]);
}

/*
 * display_channel()
 *
 * Formats one channel label and value into a short string and writes it
 * at the current cursor position.
 */
void display_channel(const char* label, uint16_t value)
{
    char buf[10];
    snprintf(buf, sizeof(buf), "%s:%u", label, value);
    lcd_putString(buf);
}

void display_rgbc(void)
{
    lcd_clear();

    lcd_moveCursor(0, 0);
    display_channel("R", red);
    lcd_moveCursor(9, 0);
    display_channel("G", green);

    lcd_moveCursor(0, 1);
    display_channel("B", blue);
    lcd_moveCursor(9, 1);
    display_channel("C", clear);
}

int main(void)
{
    i2c_init();
    lcd_init();
    lcd_enable(true);

    sensor_init();

    while (1)
    {
        read_rgbc();
        display_rgbc();
        _delay_ms(500);
    }
}