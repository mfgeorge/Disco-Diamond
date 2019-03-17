#ifndef COLOR_GENERATOR_H
#define COLOR_GENERATOR_H

#include "Arduino.h"
#include "ledstrip.h"

// Color generator function can be used to color a range.
using ColorGenerator =
    std::function<Pixel(uint16_t current_index, uint16_t total_size)>;

// Generator that generates a single color.
ColorGenerator SingleColorGenerator(const Pixel &color) {
   return [color](uint16_t, uint16_t) { return color; };
}

// Generator that generates a gradient.
ColorGenerator GradientGenerator(const Pixel &start_color,
                                 const Pixel &end_color) {
   return [start_color, end_color](uint16_t curr_point, uint16_t max_points) -> Pixel {
      return {start_color.red +
                  ((end_color.red - start_color.red) * curr_point) /
                      (max_points - 1),
              start_color.green +
                  ((end_color.green - start_color.green) * curr_point) /
                      (max_points - 1),
              start_color.blue +
                  ((end_color.blue - start_color.blue) * curr_point) /
                      (max_points - 1)};
   };
}

#endif