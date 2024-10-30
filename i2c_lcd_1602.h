#ifndef I2C_LCD_1602_H
#define I2C_LCD_1602_H

#include <avr/io.h>

void i2c_lcd_1602_init(void);
void i2c_lcd_1602_init_lcd(void);
void i2c_lcd_1602_set_cursor(uint8_t row, uint8_t col);
void i2c_lcd_1602_write_string(const char *str);
void i2c_lcd_1602_write_char(uint8_t data);
void i2c_lcd_1602_write_float_custom(float value, int decimals);  // Neue Funktion zum Schreiben von Floats
void i2c_lcd_1602_clear(void);

// Hilfsfunktionen zur Konvertierung
void i2c_lcd_1602_int_to_str(int num, char *str);
void i2c_lcd_1602_float_to_str(float num, int decimal_places, char *str);

#endif
