#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include "ledstrip.h"
#include "color_util.h"

struct Complement {
   Pixel colors[2];
};

Complement MakeComplement(int hue) {
   uint16_t h1 = HueInDegrees(hue);
   uint16_t h2 = HueInDegrees((hue + 180) % 360);
   return {HSVToRGB(h1), HSVToRGB(h2)};
}

struct Triad {
   Pixel colors[3];
};

Triad MakeTriad(int hue) {
   uint16_t h1 = HueInDegrees(hue);
   uint16_t h2 = HueInDegrees((hue + 120) % 360);
   uint16_t h3 = HueInDegrees((hue + 240) % 360);
   return {HSVToRGB(h1), HSVToRGB(h2), HSVToRGB(h3)};
}

struct Square {
   Pixel colors[4];
};

Square MakeSquare(int hue) {
   uint16_t h1 = HueInDegrees(hue);
   uint16_t h2 = HueInDegrees((hue + 90) % 360);
   uint16_t h3 = HueInDegrees((hue + 180) % 360);
   uint16_t h4 = HueInDegrees((hue + 270) % 360);
   return {HSVToRGB(h1), HSVToRGB(h2), HSVToRGB(h3), HSVToRGB(h4)};
}

struct ColorContainer{
   Pixel colors[10];
   uint8_t active_colors=0;
   uint8_t color_index=0;

   void AddColor(Pixel c){
      colors[active_colors] = c;
      active_colors++;
   }

   Pixel NextColor(){
      color_index = (color_index+1)%active_colors;
      return colors[color_index];
   }

   void ClearColors(){
      active_colors = 0;
      color_index = 0;
   }
};



#endif