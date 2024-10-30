# ATmega328P Project: MAX6675, I2C 1602 LCD, and DOG SPI Display

This project demonstrates how to interface an **ATmega328P** microcontroller with a **MAX6675 Thermocouple Sensor**, an **I2C 1602 LCD**, and a **DOG SPI Display**. It includes the necessary C and header files for controlling these components.

## Project Setup

### Files to Include

- **Header Files (`.h`):**
  - `i2c_lcd_1602.h`: Functions for controlling the I2C 1602 LCD.
  - `max6675.h`: Functions for reading temperature data from the MAX6675 sensor.
  - `dog_spi_display.h`: Functions for controlling the DOG SPI Display.

- **Source Files (`.c`):**
  - `i2c_lcd_1602.c`: Implementation of I2C LCD functions.
  - `max6675.c`: Implementation of MAX6675 communication and data reading.
  - `dog_spi_display.c`: Implementation of DOG SPI Display functions.

Include all these files in the corresponding **header** and **source** sections of your MPLAB X project.

## Pin Connections

### MAX6675 (Thermocouple Sensor)
| Signal | Pin on ATmega328P | Port |
|--------|-------------------|------|
| SCK    | PC1               | PC1  |
| CS     | PC0               | PC0  |
| SO     | PC2               | PC2  |

### I2C 1602 LCD
| Signal | Pin on ATmega328P | Port |
|--------|-------------------|------|
| SCL    | PC5 (A5)          | PC5  |
| SDA    | PC4 (A4)          | PC4  |

### DOG SPI Display
| Signal | Pin on ATmega328P | Port |
|--------|-------------------|------|
| SDA    | PD2               | PD2  |
| CLK    | PD1               | PD1  |
| CS     | PD0               | PD0  |
| RES    | PD3               | PD3  |

## Required Libraries

### 1. `i2c_lcd_1602.h`

This library handles communication with the I2C 1602 LCD. It includes functions for initializing the display, setting the cursor, writing strings, clearing the screen, and more.

### 2. `max6675.h`

This library communicates with the MAX6675 thermocouple sensor to read the temperature data over SPI.

### 3. `dog_spi_display.h`

This library handles the DOG SPI display, providing functions for sending commands and data over SPI, as well as controlling the display settings.

## Example Usage

The project includes examples for:
- Initializing and reading temperature data from the MAX6675.
- Displaying text and temperature values on the I2C 1602 LCD.
- Controlling the DOG SPI Display, including writing data, setting the cursor, and clearing the display.

## Additional Notes

- **F_CPU**: Ensure that the `F_CPU` macro is correctly defined in your project to reflect the clock frequency you are using.
- **Delays**: Use appropriate delays to ensure that the devices have enough time to process commands.
  
## How to Run

1. Connect the devices to the ATmega328P according to the pin mapping provided above.
2. Compile the project using MPLAB X.
3. Program the ATmega328P using an appropriate programmer (e.g., USBasp or AVR ISP).
4. Once programmed, the LCD will display "Hello World," and the MAX6675 will start reading and displaying the temperature.

---

### License

This project is open-source under the [MIT License](LICENSE).
