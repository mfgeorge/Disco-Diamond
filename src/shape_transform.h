#ifndef SHAPE_TRANSFORM_H
#define SHAPE_TRANSFORM_H

#include "Arduino.h"
#include "ledstrip.h"
#include "pattern.h"

class VerticalShapeTransform {
 public:
   bool NormalizeRange(const LinearRange &range,
                       LinearRange *normalized_range) {
      if (range.start > range.end) {
         Serial.println("ERROR: drawing failed because start bigger than end");
         return false;
      }

      int16_t normalized_start = range.start;
      int16_t normalized_end = range.end;

      if (range.IsWrapped()) {
         // If we are wrapping, normalize the range.
         // For some reason normalized_start % (RANGE_MAX + 1) doesnt work here.
         while (normalized_start < 0) {
            normalized_start += RANGE_MAX + 1;
         }
         normalized_end = normalized_start + range.Length() - 1;
      } else {
         if (normalized_start > RANGE_MAX) {
            Serial.println("drawing failed because out of bounds");
            return false;
         }
         // If we are not wrapping, trim range to bounds.
         normalized_start = normalized_start < 0 ? 0 : normalized_start;
         normalized_end =
             normalized_end > RANGE_MAX ? RANGE_MAX : normalized_end;
      }
      *normalized_range = range;
      normalized_range->start = normalized_start;
      normalized_range->end = normalized_end;
      return true;
   }

   void DrawRange(const LinearRange &range, Pixel *hw_pixels,
                  uint16_t hw_pixel_count) {
      LinearRange normalized_range;
      // Make sure range is normalized for direct pixel mapping.
      if (!NormalizeRange(range, &normalized_range)) {
         // Return if we could not normalize it properly.
         return;
      }

      uint16_t start_pixel =
          (hw_pixel_count * normalized_range.start) / RANGE_MAX;
      uint16_t end_pixel = (hw_pixel_count * normalized_range.end) / RANGE_MAX;

      if (end_pixel < start_pixel) {
         // This should never happen.
         Serial.println("ERROR: Start pixel was greater than end pixel.");
         return;
      }

      uint16_t pixels_to_draw = end_pixel - start_pixel + 1;
      for (uint16_t p = start_pixel; p <= end_pixel; p++) {
         hw_pixels[p % hw_pixel_count] =
             range.color_generator(p - start_pixel, pixels_to_draw);
      }
   }

   void Transform(const PatternFrame *patternFrame, HwFrame *hwFrame) {
      for (int i = 0; i < hwFrame->getStripCount(); i++) {
         Pixel *hw_pixels = hwFrame->getMutablePixels(i);
         uint16_t hw_pixel_count = hwFrame->getNumPixels(i);
         // Default is the pixel is off
         memset(hw_pixels, 0, hw_pixel_count * sizeof(Pixel));
         const PatternFrameDimension *curr_dimension =
             &patternFrame->dimensions[0];
         for (int r = 0; r < curr_dimension->rangeCount; r++) {
            DrawRange(curr_dimension->ranges[r], hw_pixels, hw_pixel_count);
         }
      }
   }
};

#endif