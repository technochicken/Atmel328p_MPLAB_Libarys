
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "EA-DOGS164W-A.h"


#define F_CPU 4000000UL  // System clock frequency (16 MHz for ATmega328P)
#define SCL_CLOCK 100000L // I2C clock frequency (100 kHz)

// Initialize I2C (TWI) interface
void i2c_init() {
    TWSR = 0x00; // Set prescaler to 1
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; // Set I2C clock rate
}

// Send I2C start condition
bool i2c_start(uint8_t address) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); // Send start condition
    while (!(TWCR & (1 << TWINT)));                   // Wait for completion

    TWDR = address;                                   // Load slave address
    TWCR = (1 << TWEN) | (1 << TWINT);                // Clear TWINT to start transmission
    while (!(TWCR & (1 << TWINT)));                   // Wait for completion

    // Check if device acknowledged the address
    uint8_t status = TWSR & 0xF8;
    if (status == 0x18 || status == 0x40) { // SLA+W or SLA+R acknowledged
        return true;
    } else {
        return false;
    }
}

// Send I2C stop condition
void i2c_stop() {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); // Send stop condition
}

// I2C port scanner
void i2c_port_scanner() {
    for (uint8_t address = 0x03; address < 0x78; address++) {
        if (i2c_start((address << 1))) {
            // Device found at this address
            lcd_set_cursor(1, 0);              // Setzt den Cursor auf die zweite Zeile, erste Spalte
            lcd_write_int(address);       // Schreibt "Fehler!" auf das Display
            i2c_stop();
            // You can implement the output method here (e.g., send data via UART, LCD, etc.)
            // For this example, we will just blink an LED when a device is found
            PORTB |= (1 << PB0);  // Turn LED on
            _delay_ms(100);       // Wait 100 ms
            PORTB &= ~(1 << PB0); // Turn LED off
            _delay_ms(400);       // Wait 400 ms
        }
        i2c_stop(); // Always send stop after each scan attempt
    }
}

int portscannermain() {
    // Initialize I2C
    pin_setup();       // Setze die Pin-Modi
    lcd_setup();       // Führe das LCD-Setup durch
    lcd_clear();                       // Löscht das Display
    lcd_set_display(true, false, false); // Display aktivieren, Cursor und Blinken deaktivieren
    lcd_clear();
    lcd_set_cursor(0, 0);              // Setzt den Cursor auf die zweite Zeile, erste Spalte
    lcd_write_string("Scanne!");       // Schreibt "Fehler!" auf das Display
    i2c_init();

    // Set PB0 as output (for LED)
    DDRB |= (1 << PB0);

    while (1) {
        i2c_port_scanner(); // Scan I2C addresses
        _delay_ms(2000);    // Wait 2 seconds before rescanning
    }

    return 0;
}
