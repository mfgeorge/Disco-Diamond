#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino_NeoPixel.h>

// code info https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
// these are the led's we used for the snowglobe 2019 totem pole: https://www.adafruit.com/product/1461

struct pixel {
    int red;                // 0 to 255 
    int blue;               // 0 to 255
    int green;              // 0 to 255
}

struct ledStrip {
    Arduino_NeoPixel strip; // the object will be defined during struct instantiation
    int radialPos;          // 0 to 360 degrees
    pixel pixels[100]       // array of pixels, max size 100 for now
}

struct shapedFrame {
    // GPIO pin mapping from MCU to header pins is defined on this schematic:
    // https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF
    // GPIO16 = D0
    // GPIO5 = D1
    // GPIO4 = D2
    // GPIO0 = D3
    ledFrame strip0Deg {.strip = Arduino_NeoPixel(42, 16), .radialPos = 0}
    ledFrame strip90Deg {.strip = Arduino_NeoPixel(42, 5), .radialPos = 90}
    ledFrame strip180Deg {.strip = Arduino_NeoPixel(42, 4), .radialPos = 180}
    ledFrame strip270Deg {.strip = Arduino_NeoPixel(42, 0), .radialPos = 270}
    int isWriting;
}

void updateStripColors(ledStrip strip, int reds[], int blues[], int greens[])

#endif