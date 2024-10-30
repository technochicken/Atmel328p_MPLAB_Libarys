#ifndef MAX6675_H
#define MAX6675_H

#include <avr/io.h>

// Pin Definitionen
#define CS_MAX     PC0
#define SCK_MAX    PC1
#define SO_MAX     PC2

// Funktionsprototypen
void max6675_init_pins(void);
float max6675_read_temperature(void);

#endif  // MAX6675_H
