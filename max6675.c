
#include "max6675.h"
#include <util/delay.h>
#include <stdbool.h>

// Initialisiere die Pins
void max6675_init_pins() {
    // Setze CS und SCK als Ausgänge und SO als Eingang
    DDRC |= (1 << CS_MAX) | (1 << SCK_MAX);   // CS und SCK als Ausgang
    DDRC &= ~(1 << SO_MAX);                   // SO als Eingang
    
    // Setze Standardwerte: CS High (inaktiv) und SCK Low
    PORTC |= (1 << CS_MAX);                   // CS High (inaktiv)
    PORTC &= ~(1 << SCK_MAX);                 // SCK Low
}

// Funktion, um eine kleine Verzögerung für SPI zu erzeugen
static void spi_delay() {
    _delay_us(10);  // Kurze Verzögerung (einstellbar je nach Timing-Anforderung)
}

// Funktion, um die rohen Daten vom MAX6675 zu lesen
static uint16_t max6675_read_raw_data() {
    uint16_t data = 0;

    // Setze CS auf LOW, um die Übertragung zu starten
    PORTC &= ~(1 << CS_MAX);

    // Lese 16-Bit-Daten vom MAX6675
    for (int i = 15; i >= 0; --i) {
        // Setze SCK auf HIGH, um ein Bit zu lesen
        PORTC |= (1 << SCK_MAX);
        spi_delay();

        // Lese das Bit von SO (Serial Output)
        if (PINC & (1 << SO_MAX)) {
            data |= (1 << i);
        }

        // Setze SCK auf LOW, um den Lesevorgang abzuschließen
        PORTC &= ~(1 << SCK_MAX);
        spi_delay();
    }

    // Setze CS auf HIGH, um die Übertragung zu beenden
    PORTC |= (1 << CS_MAX);

    return data;
}

// Funktion, um die Temperatur vom MAX6675 zu lesen und als float zurückzugeben
float max6675_read_temperature() {
    uint16_t raw_data = max6675_read_raw_data();

    // Überprüfe, ob das Thermoelement nicht angeschlossen ist (Bit 2 = 1 bei Fehler)
    if (raw_data & 0x4) {
        return -1;  // Fehler: Thermoelement nicht angeschlossen
    }

    // Entferne die Fehlerbits und das Dummy-Bit (die letzten 3 Bits)
    raw_data >>= 3;

    // Umwandlung in Temperatur: 0.25°C pro LSB
    float thermocouple_temperature = raw_data * 0.25;

    // Kalibrierung basierend auf Umgebungsfaktoren (optional)
    thermocouple_temperature += (-5.5);  // Beispielkalibrierung

    return thermocouple_temperature;
}
