#include "EA-DOGS164W-A.h"
#include <util/delay.h>

// Hilfsfunktionen zur Konvertierung
void int_to_str(int num, char *str) {
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

void float_to_str(float num, int decimal_places, char *str) {
    int int_part = (int)num;
    float frac_part = num - (float)int_part;
    int i = 0;

    if (num < 0) {
        str[i++] = '-';
        int_part = -int_part;
        frac_part = -frac_part;
    }

    int_to_str(int_part, &str[i]);
    while (str[i] != '\0') {
        i++;
    }

    str[i++] = '.';

    for (int j = 0; j < decimal_places; j++) {
        frac_part *= 10;
    }

    int_to_str((int)frac_part, &str[i]);
}

// Pin Setup
void pin_setup(void) {
    DDRD |= (1 << CS) | (1 << CLK) | (1 << DATAIN) | (1 << RESET);
    PORTD |= (1 << CS) | (1 << CLK) | (1 << DATAIN) | (1 << RESET);
}

// LCD Funktionen
void lcd_send_byte(uint8_t byte, bool R_W, bool RS) {
    uint8_t bit_pattern[24] = {0};

    bit_pattern[0] = 1;
    bit_pattern[1] = 1;
    bit_pattern[2] = 1;
    bit_pattern[3] = 1;
    bit_pattern[4] = 1;
    bit_pattern[5] = (R_W ? 1 : 0);
    bit_pattern[6] = (RS ? 1 : 0);

    bit_pattern[8]  = ((byte & (1 << 0)) ? 1 : 0);
    bit_pattern[9]  = ((byte & (1 << 1)) ? 1 : 0);
    bit_pattern[10] = ((byte & (1 << 2)) ? 1 : 0);
    bit_pattern[11] = ((byte & (1 << 3)) ? 1 : 0);

    bit_pattern[16] = ((byte & (1 << 4)) ? 1 : 0);
    bit_pattern[17] = ((byte & (1 << 5)) ? 1 : 0);
    bit_pattern[18] = ((byte & (1 << 6)) ? 1 : 0);
    bit_pattern[19] = ((byte & (1 << 7)) ? 1 : 0);

    PORTD &= ~(1 << CS);

    for (int i = 0; i < 24; i++) {
        if (bit_pattern[i]) {
            PORTD |= (1 << DATAIN);
        } else {
            PORTD &= ~(1 << DATAIN);
        }

        _delay_us(10);  
        PORTD &= ~(1 << CLK);
        _delay_us(20);
        PORTD |= (1 << CLK);
        _delay_us(20);
    }

    PORTD |= (1 << CS);
}

void lcd_setup(void) {
    PORTD &= ~(1 << RESET);
    _delay_ms(100);
    PORTD |= (1 << RESET);
    _delay_ms(100);

    _delay_ms(5);   
    lcd_send_byte(0b00111110, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00001001, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00000110, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00011110, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00111001, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00010011, false, false);
    _delay_ms(5);  
    lcd_send_byte(0b01101100, false, false);
    _delay_ms(5);
    lcd_send_byte(0b01011100, false, false);
    _delay_ms(5);
    lcd_send_byte(0b01110000, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00111000, false, false);
    _delay_ms(5);
    lcd_send_byte(0b00001100, false, false);
    _delay_ms(5);
}

void lcd_write_char(char c) {
    lcd_send_byte(c, false, true);
}

void lcd_write_string(const char *str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}

void lcd_write_int(int num) {
    char buffer[12];
    int_to_str(num, buffer);
    lcd_write_string(buffer);
}

void lcd_write_float(float num, int decimal_places) {
    char buffer[20];
    float_to_str(num, decimal_places, buffer);
    lcd_write_string(buffer);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t position = 0x80;

    if (row == 0) {
        position += col;
    } else if (row == 1) {
        position += 0x40 + col;
    }

    lcd_send_byte(position, false, false);
}

void lcd_set_display(bool display_on, bool cursor_on, bool blink_on) {
    uint8_t command = 0x08;

    if (display_on) {
        command |= 0x04;
    }
    if (cursor_on) {
        command |= 0x02;
    }
    if (blink_on) {
        command |= 0x01;
    }

    lcd_send_byte(command, false, false);
}

void lcd_clear(void) {
    lcd_send_byte(0x01, false, false);
    _delay_ms(2);
}

void lcd_home(void) {
    lcd_send_byte(0x02, false, false);
    _delay_ms(2);
}
