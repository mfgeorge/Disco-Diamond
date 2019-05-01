#ifndef SHAPE_TRANSFORM_H
#define SHAPE_TRANSFORM_H

#include "Arduino.h"
#include "ledstrip.h"
#include "pattern.h"

#define DEBUG 0

// Blends two colors with proportion blend_percent of c1.
// blend_percent is 0 to 100.
// blend_percent = 50 implies balanced blending.
Pixel Blend(Pixel c1, Pixel c2, uint16_t blend_percent) {
   return Pixel{
       (c1.red * blend_percent + c2.red * (100 - blend_percent)) / 100,
       (c1.green * blend_percent + c2.green * (100 - blend_percent)) / 100,
       (c1.blue * blend_percent + c2.blue * (100 - blend_percent)) / 100};
}

class VerticalShapeTransform {
 public:
   bool NormalizeRange(const LinearRange& range,
                       LinearRange* normalized_range) {
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
            // Serial.println("drawing failed because out of bounds");
            return false;
         }
         // If we are not wrapping, trim range to bounds.
         normalized_start = normalized_start < 0 ? 0 : normalized_start;
         if (normalized_end < 0) {
            normalized_end = 0;
         } else if (normalized_end > RANGE_MAX) {
            normalized_end = RANGE_MAX;
         }
      }
      *normalized_range = range;
      normalized_range->start = normalized_start;
      normalized_range->end = normalized_end;
#if DEBUG
      Serial.printf("...normalized to (%d, %d)...", normalized_start,
                    normalized_end);
#endif
      return true;
   }

   void DrawRange(const LinearRange& range, Pixel* hw_pixels,
                  uint16_t hw_pixel_count) {
      if (!range.IsVisible()) {
         return;
      }
      LinearRange normalized_range;
      // Make sure range is normalized for direct pixel mapping.
      if (!NormalizeRange(range, &normalized_range)) {
         // Return if we could not normalize it properly.
         return;
      }

      uint16_t start_pixel =
          (hw_pixel_count * normalized_range.start) / (RANGE_MAX + 1);
      uint16_t end_pixel =
          (hw_pixel_count * normalized_range.end) / (RANGE_MAX + 1);
#if DEBUG
      Serial.printf("...mapped to pixels [%d, %d]/%d...", start_pixel,
                    end_pixel, hw_pixel_count);
#endif
      if (end_pixel < start_pixel) {
         // This should never happen.
         Serial.println("ERROR: Start pixel was greater than end pixel.");
         return;
      }

#if DEBUG
      Serial.printf("...trimmed to pixels [%d, %d]/%d...", start_pixel,
                    end_pixel, hw_pixel_count);
#endif

      uint16_t pixels_to_draw = end_pixel - start_pixel + 1;
      if (range.Dithered()) {
         // Dither the start pixel
         auto existing_color = hw_pixels[start_pixel % hw_pixel_count];
         auto base_color = range.color_generator(0, pixels_to_draw);
         auto remainder = (range.Start() * hw_pixel_count) % RANGE_MAX;
         hw_pixels[start_pixel % hw_pixel_count] =
             Blend(base_color, existing_color,
                   (RANGE_MAX - remainder) * 100 / RANGE_MAX);

         // Color all pixels except the edges
         for (uint16_t p = start_pixel + 1; p < end_pixel; p++) {
            hw_pixels[p % hw_pixel_count] =
                range.color_generator(p - start_pixel - 1, pixels_to_draw - 2);
         }

         // Dither the end pixel
         existing_color = hw_pixels[end_pixel % hw_pixel_count];
         base_color = range.color_generator(pixels_to_draw - 1, pixels_to_draw);
         remainder = (range.End() * hw_pixel_count) % RANGE_MAX;
         hw_pixels[end_pixel % hw_pixel_count] =
             Blend(base_color, existing_color,
                   100 - ((RANGE_MAX - remainder) * 100 / RANGE_MAX));
      } else {
         for (uint16_t p = start_pixel; p <= end_pixel; p++) {
            hw_pixels[p % hw_pixel_count] =
                range.color_generator(p - start_pixel, pixels_to_draw);
         }
      }
#if DEBUG
      Serial.printf("...success...");
#endif
   }

   void Transform(const PatternFrame* patternFrame, HwFrame* hwFrame) {
      Pixel* hw_pixels = hwFrame->getMutablePixels(0);
      uint16_t hw_pixel_count = hwFrame->getNumPixels(0);
      // Default is the pixel is off
      memset(hw_pixels, 0, hw_pixel_count * sizeof(Pixel));
      const PatternDimension* curr_dimension = &patternFrame->dimensions[0];
      for (int r = 0; r < curr_dimension->rangeCount; r++) {
#if DEBUG
         Serial.printf("\r\ndrawing layer %d from %d to %d...", r,
                       curr_dimension->ranges[r].start,
                       curr_dimension->ranges[r].end);
#endif
         DrawRange(curr_dimension->ranges[r], hw_pixels, hw_pixel_count);
      }

      for (int i = i; i < hwFrame->getStripCount(); i++) {
         memcpy(hwFrame->getMutablePixels(i), hw_pixels,
                hw_pixel_count * sizeof(Pixel));
      }
   }
};

#endif