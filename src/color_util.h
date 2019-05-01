#ifndef COLOR_UTIL_H
#define COLOR_UTIL_H

#include "Arduino.h"

static const uint8_t PROGMEM _GammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

uint8_t gamma(uint8_t x) {
   return pgm_read_byte(&_GammaTable[x]);
}

Pixel gamma(Pixel color) {
   return Pixel{gamma(color.red), gamma(color.green), gamma(color.blue)};
}

Pixel HSVToRGB(uint16_t hue, uint8_t sat = 255, uint8_t val = 255) {
   uint8_t r, g, b;
   hue = (hue * 1530L + 32768) / 65536;
   if (hue < 510) { // Red to Green-1
      b = 0;
      if (hue < 255) { //   Red to Yellow-1
         r = 255;
         g = hue;       //     g = 0 to 254
      } else {          //   Yellow to Green-1
         r = 510 - hue; //     r = 255 to 1
         g = 255;
      }
   } else if (hue < 1020) { // Green to Blue-1
      r = 0;
      if (hue < 765) { //   Green to Cyan-1
         g = 255;
         b = hue - 510;  //     b = 0 to 254
      } else {           //   Cyan to Blue-1
         g = 1020 - hue; //     g = 255 to 1
         b = 255;
      }
   } else if (hue < 1530) { // Blue to Red-1
      g = 0;
      if (hue < 1275) {  //   Blue to Magenta-1
         r = hue - 1020; //     r = 0 to 254
         b = 255;
      } else { //   Magenta to Red-1
         r = 255;
         b = 1530 - hue; //     b = 255 to 1
      }
   } else { // Last 0.5 Red (quicker than % operator)
      r = 255;
      g = b = 0;
   }

   // Apply saturation and value to R,G,B, pack into 32-bit result:
   uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
   uint16_t s1 = 1 + sat;  // 1 to 256; same reason
   uint8_t s2 = 255 - sat; // 255 to 0
   return {((((r * s1) >> 8) + s2) * v1) >> 8,
           ((((g * s1) >> 8) + s2) * v1) >> 8,
           ((((b * s1) >> 8) + s2) * v1) >> 8};
}

#endif