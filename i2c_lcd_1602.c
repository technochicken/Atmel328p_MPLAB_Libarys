#include "i2c_lcd_1602.h"
#include <util/delay.h>
#include <stdbool.h>

#define F_CPU 4000000UL
#define SCL_CLOCK 100000L

#define I2C_LCD_1602_ADDRESS 0x27
#define I2C_LCD_1602_BACKLIGHT 0x08
#define I2C_LCD_1602_ENABLE 0x04

// Initialisiere I²C-Schnittstelle
void i2c_lcd_1602_init(void) {
    uint16_t bitrate = ((F_CPU / SCL_CLOCK) - 16) / 2;
    if (bitrate <= 255) {
        TWBR = (uint8_t)bitrate;
    } else {
        TWBR = 255;
    }
}

void i2c_lcd_1602_start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_lcd_1602_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void i2c_lcd_1602_write_byte(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_lcd_1602_send(uint8_t data, uint8_t mode) {
    uint8_t high_nibble = (data & 0xF0) | I2C_LCD_1602_BACKLIGHT | mode;
    uint8_t low_nibble = ((data << 4) & 0xF0) | I2C_LCD_1602_BACKLIGHT | mode;

    i2c_lcd_1602_start();
    i2c_lcd_1602_write_byte(I2C_LCD_1602_ADDRESS << 1);
    i2c_lcd_1602_write_byte(high_nibble | I2C_LCD_1602_ENABLE);
    i2c_lcd_1602_write_byte(high_nibble & ~I2C_LCD_1602_ENABLE);
    i2c_lcd_1602_stop();

    i2c_lcd_1602_start();
    i2c_lcd_1602_write_byte(I2C_LCD_1602_ADDRESS << 1);
    i2c_lcd_1602_write_byte(low_nibble | I2C_LCD_1602_ENABLE);
    i2c_lcd_1602_write_byte(low_nibble & ~I2C_LCD_1602_ENABLE);
    i2c_lcd_1602_stop();
}

void i2c_lcd_1602_send_command(uint8_t command) {
    i2c_lcd_1602_send(command, 0x00);
}

void i2c_lcd_1602_write_char(uint8_t data) {
    i2c_lcd_1602_send(data, 0x01);
}

void i2c_lcd_1602_init_lcd() {
    _delay_ms(50);

    i2c_lcd_1602_send_command(0x33);
    i2c_lcd_1602_send_command(0x32);
    i2c_lcd_1602_send_command(0x28);
    i2c_lcd_1602_send_command(0x0C);
    i2c_lcd_1602_send_command(0x06);
    i2c_lcd_1602_send_command(0x01);
    _delay_ms(5);
}

void i2c_lcd_1602_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? 0x80 + col : 0xC0 + col;
    i2c_lcd_1602_send_command(address);
}

void i2c_lcd_1602_write_string(const char *str) {
    while (*str) {
        i2c_lcd_1602_write_char(*str++);
    }
}

// Funktion zur Anzeige von Float-Werten mit Hilfsfunktionen
void i2c_lcd_1602_write_float_custom(float value, int decimals) {
    char buffer[10];
    i2c_lcd_1602_float_to_str(value, decimals, buffer);
    i2c_lcd_1602_write_string(buffer);
}

// Implementierung von Hilfsfunktionen zur Konvertierung
void i2c_lcd_1602_int_to_str(int num, char *str) {
    int i = 0;
    bool is_negative = false;

    if (num < 0) {
        is_negative = true;
        num = -num;
    }

    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num != 0);

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void i2c_lcd_1602_float_to_str(float num, int decimal_places, char *str) {
    int int_part = (int)num;
    float frac_part = num - (float)int_part;
    int i = 0;

    if (num < 0) {
        str[i++] = '-';
        int_part = -int_part;
        frac_part = -frac_part;
    }

    i2c_lcd_1602_int_to_str(int_part, &str[i]);
    while (str[i] != '\0') {
        i++;
    }

    str[i++] = '.';

    for (int j = 0; j < decimal_places; j++) {
        frac_part *= 10;
    }

    i2c_lcd_1602_int_to_str((int)frac_part, &str[i]);
}


void i2c_lcd_1602_clear(void) {
    i2c_lcd_1602_send_command(0x01);  // Send the clear display command
    _delay_ms(2);  // A small delay to ensure the display is cleared
}
