
#include <avr/io.h>
#include <util/delay.h>
#include "EA-DOGS164W-A.h"
#include "max6675.h"

int examplemain(void) {
    // LCD Initialisierung
    pin_setup();       // Setze die Pin-Modi
    lcd_setup();       // Führe das LCD-Setup durch
    
    lcd_clear();                       // Löscht das Display
    lcd_set_display(true, false, false); // Display aktivieren, Cursor und Blinken deaktivieren

    // MAX6675 Initialisierung
    max6675_init_pins();  // Setze die Pins für den MAX6675

    while (1) {
        // Lese die Temperatur vom MAX6675
        float temperature = max6675_read_temperature();

        if (temperature < 0) {
            // Fehler: Thermoelement nicht angeschlossen
            lcd_clear();
            lcd_set_cursor(0, 0);              // Setzt den Cursor auf die zweite Zeile, erste Spalte
            lcd_write_string("Fehler!");       // Schreibt "Fehler!" auf das Display
        } else {
            lcd_set_cursor(0, 0);              // Setzt den Cursor auf die erste Zeile, erste Spalte
            lcd_write_string("Temperatur:");         // Schreibt "Temp:" auf das Display
            lcd_set_cursor(1, 0);              // Setzt den Cursor auf die zweite Zeile, erste Spalte
            lcd_write_float(temperature, 2);   // Temperatur mit 2 Dezimalstellen anzeigen
            lcd_write_char(223);               // Gradzeichen
            lcd_write_char('C');               // "C" für Celsius
            lcd_write_char(' ');
            lcd_write_char(' ');
            lcd_write_char(' ');
            lcd_write_char(' ');
        }

        _delay_ms(1000);  // 1 Sekunde warten, bevor die Temperatur erneut gelesen wird
    }

    return 0;
}
