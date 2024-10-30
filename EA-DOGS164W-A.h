#ifndef EA_DOGS164W_A_H
#define EA_DOGS164W_A_H

#include <avr/io.h>
#include <stdbool.h>

// Pin Definitionen  ( nur mit pins auf PD funktional!!!)
#define CS     PD0
#define CLK    PD1
#define DATAIN PD2
#define RESET  PD3
#define Sendfrequency 10  // Sende-Frequenz in kHz

// Funktionsdeklarationen
void pin_setup(void);
void lcd_setup(void);
void lcd_send_byte(uint8_t byte, bool R_W, bool RS);
void lcd_write_char(char c);
void lcd_write_string(const char *str);
void lcd_write_int(int num);
void lcd_write_float(float num, int decimal_places);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_set_display(bool display_on, bool cursor_on, bool blink_on);
void lcd_clear(void);
void lcd_home(void);

// Hilfsfunktionen
void int_to_str(int num, char *str);
void float_to_str(float num, int decimal_places, char *str);

#endif  // EA_DOGS164W_A_H
