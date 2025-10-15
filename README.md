# Arduino 0.96" OLED Game of Life

Arduino program that runs Conway's Game of Life using an Arduino Nano and a 0.96-inch OLED display.

**By: Grant Oliver**  
**Published: Oct. 15, 2025**

---

## Overview

This program is optimized to work within the Arduino Nano's limited memory using [bit packing](https://www.cs.cornell.edu/courses/cs3410/2024fa/notes/bitpack.html).

### Hardware
- Arduino Nano  
- 0.96" I²C OLED Display  

### Libraries
- Adafruit GFX  
- Adafruit SSD1306  
- Arduino Wire (for I²C communication)

---

## Instructions

Wire the OLED display using a breadboard and jumper cables, or connect via a PCB board.

### 1. OLED Wiring
| OLED Pin | Arduino Pin |
|-----------|--------------|
| GND | GND |
| VCC | 5V |
| SCL | A5 |
| SDA | A4 |

---

### 2. Install Adafruit Libraries
Use [this resource from Adafruit](https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples) to install the appropriate libraries for integrating OLED displays.

---

### 3. Upload the Code
Upload the file `arduino-096oled-game-of-life.ino` to your Arduino Nano board using the Arduino IDE.
