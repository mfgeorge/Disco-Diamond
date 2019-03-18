#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "color_generator.h"
#include "ledstrip.h"

#define RANGE_MAX 1023
#define MAX_RANGES 10
#define MAX_DIMENSIONS 5

enum AnchorType {
   FRONT,
   TAIL,
   CENTER,
};

class AbstractLinearRange {
 public:
   int16_t start;
   int16_t end;

   // Indicates whether or not this should be wrapped across the dimension if it
   // extends past the end.
   bool wrapped;

   AbstractLinearRange() : wrapped(false) {}

   AbstractLinearRange(int16_t start, int16_t end)
       : start(start), end(end), wrapped(false) {}

   void Wrap() { wrapped = true; }

   bool IsWrapped() const { return wrapped; }

   int16_t Center() const { return (end - start) / 2; }

   uint16_t Length() const { return end - start + 1; }

   void Slide(int16_t increment) {
      int16_t length = Length();
      start = (start + increment) % RANGE_MAX;
      end = start + length - 1;
   }

   void SetAbsoluteLength(uint16_t length, AnchorType anchor_type = FRONT) {
      switch (anchor_type) {
      case FRONT:
         end = start + (int16_t)length;
         break;
      case TAIL:
         start = end - (int16_t)length;
         break;
      case CENTER:
         int16_t center = Center();
         start = center - (length / 2);
         end = center + (length / 2) + (length % 2);
         break;
      }
   }

   void Stretch(float stretch_factor, AnchorType anchor_type = FRONT) {
      int16_t stretched_length = Length() * stretch_factor;
      SetAbsoluteLength(stretched_length, anchor_type);
   }

   void Normalize() {
      while (start > end) {
         end += (RANGE_MAX + 1);
      }
   }
};

class LinearRange : public AbstractLinearRange {
 public:
   Pixel start_color;
   Pixel end_color;
   ColorGenerator color_generator;

   // Creates a solid color range.
   static LinearRange SolidColor(int16_t start, int16_t end, Pixel color) {
      return LinearRange(start, end, SingleColorGenerator(color));
   }

   // Creates a gradient range.
   static LinearRange Gradient(int16_t start, int16_t end, Pixel start_color,
                               Pixel end_color) {
      return LinearRange(start, end, GradientGenerator(start_color, end_color));
   }

   // Empty linear range will not draw anything and exists for convenience.
   LinearRange() : LinearRange(0, 0, SingleColorGenerator(Pixel{})) {}

   // Generates colors along the parent dimension from start to end using
   // color_generator.
   LinearRange(int16_t start, int16_t end, ColorGenerator color_generator)
       : AbstractLinearRange(start, end), color_generator(color_generator) {}
};

class PatternDimension {
 public:
   LinearRange ranges[MAX_RANGES];
   uint8_t rangeCount;

   LinearRange* AddLayer() {
      uint8_t new_range_index = rangeCount;
      rangeCount++;
      return &ranges[new_range_index];
   }
};

class PatternFrame {
 public:
   PatternDimension dimensions[MAX_DIMENSIONS];
   uint8_t activeDimensions;

   PatternDimension* AddDimension() {
      uint8_t new_dimension_index = activeDimensions;
      activeDimensions++;
      return &dimensions[new_dimension_index];
   }
};

#endif