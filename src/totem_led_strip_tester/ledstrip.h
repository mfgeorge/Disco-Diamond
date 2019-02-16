#IFNDEF LEDSTRIP_H
#DEFINE LEDSTRIP_H

#include <Arduino_NeoPixel.h>

// code info https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
// these are the led's we used for the snowglobe 2019 totem pole: https://www.adafruit.com/product/1461
struct pixel {
    int red;            // 0 to 255 
    int blue;           // 0 to 255
    int green;          // 0 to 255
}

struct ledStrip {
    int len;            // 0 to n # of serial LEDs on the same strip
    int pin;            // the physical pin on the MCU board the LED data signal is attached to
    int radialPos;      // 0 to 360 degrees
    int brightness;     // 0 to 255. not intended for constant use due to poor performance. see:
                        // https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
    pixel pixels[100]   // array of pixels, max size 100 for now
}

struct shapedFrame {
    ledFrame strip_0_degrees {42, 0, }
    ledFrame 
}

#ENDIF