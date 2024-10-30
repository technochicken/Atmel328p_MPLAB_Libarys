#include <avr/io.h>
#include <util/delay.h>
#include "i2c_lcd_1602.h"
#include "max6675.h"

int lcd_max6675_eample_main(void) {
    // I²C und LCD initialisieren
    i2c_lcd_1602_init();
    i2c_lcd_1602_init_lcd();

    // MAX6675 Initialisierung
    max6675_init_pins();

    while (1) {
        // Temperatur vom MAX6675 lesen
        float temperature = max6675_read_temperature();
        i2c_lcd_1602_clear();

        if (temperature < 0) {
            i2c_lcd_1602_set_cursor(0, 0);
            i2c_lcd_1602_write_string("Fehler: Keine Temp");
        } else {
            i2c_lcd_1602_set_cursor(0, 0);
            i2c_lcd_1602_write_string("Temperatur:");
            i2c_lcd_1602_set_cursor(1, 0);
            i2c_lcd_1602_write_float_custom(temperature, 2);  // Temperatur mit 2 Dezimalstellen anzeigen
            i2c_lcd_1602_write_char(223);  // Gradzeichen
            i2c_lcd_1602_write_char('C');  // "C" für Celsius
        }

        _delay_ms(1000);
    }

    return 0;
}
