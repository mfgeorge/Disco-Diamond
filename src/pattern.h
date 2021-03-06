#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "color_generator.h"
#include "ledstrip.h"

#define RANGE_MAX 1023
#define MAX_RANGES 20
#define MAX_DIMENSIONS 4

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

   bool visible_;

   AbstractLinearRange() : wrapped(false), visible_(true) {}

   AbstractLinearRange(int16_t start, int16_t end)
       : start(start), end(end), wrapped(false), visible_(true) {}

   void SetVisible(bool visible) { visible_ = visible; }
   inline bool IsVisible() const { return visible_; }

   void Wrap() { wrapped = true; }
   inline bool IsWrapped() const { return wrapped; }

   inline int16_t Start() const { return start; }
   inline int16_t End() const { return end; }
   inline int16_t Center() const { return (end - start) / 2; }
   inline uint16_t Length() const { return end - start + 1; }

   void Slide(int16_t increment) {
      int16_t length = Length();
      start = start + increment;
      if (wrapped) {
         start = start % RANGE_MAX;
      }
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

   void SetPosition(int16_t position, AnchorType anchor_type = FRONT) {
      auto length = Length();
      switch (anchor_type) {
      case FRONT:
         start = position;
         end = position + length - 1;
      case TAIL:
         end = position;
         start = end - length + 1;
      case CENTER:
         start = position - length / 2;
         end = start + length - 1;
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
   bool dithered;
   bool gamma_correction_;
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
       : AbstractLinearRange(start, end), dithered(false),
         color_generator(color_generator) {}

   inline bool Dithered() const { return dithered; }
   void EnableDithering() { dithered = true; }

   void EnableGammaCorrection() {
      gamma_correction_ = true;
   }

   inline bool UseGammaCorrection() const {
      return gamma_correction_;
   }
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

   void ClearLayers() {
      rangeCount = 0;
   }
};

class PatternFrame {
 public:
   PatternDimension dimensions[MAX_DIMENSIONS];
   uint8_t activeDimensions;

   PatternDimension* AddDimension() {
      uint8_t new_dimension_index = activeDimensions;
      activeDimensions++;
      dimensions[new_dimension_index].ClearLayers();
      return &dimensions[new_dimension_index];
   }

   void ClearDimensions() {
      activeDimensions = 0;
   }
};

class Pattern {
 public:
   // Move the pattern forward one step
   virtual void step() = 0;

   // Prepare to draw on the specified pattern_frame
   virtual void init(PatternFrame* pattern_frame) = 0;
   
   // Delay in ms between frames
   virtual uint16_t frame_delay() = 0;
};

#endif