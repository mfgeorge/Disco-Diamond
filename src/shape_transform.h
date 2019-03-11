#ifndef SHAPE_TRANSFORM_H
#define SHAPE_TRANSFORM_H

#include "Arduino.h"
#include "ledstrip.h"
#include "pattern.h"

#define VERICAL_SHAPE_TRANSFORM_MAX_DIMS 4

class VerticalShapeTransform {
public:
    Pixel ComputeGradientIntersect(uint16_t curr_point, uint16_t max_points, const Pixel& start_color, const Pixel& end_color) {
        return {
            start_color.red + ((end_color.red - start_color.red) * curr_point) / (max_points - 1),
            start_color.green + ((end_color.green - start_color.green) * curr_point) / (max_points - 1),
            start_color.blue + ((end_color.blue - start_color.blue) * curr_point) / (max_points - 1)
        };
    }

    Pixel ComputePixelColor(const PatternFrameRange& range, uint16_t curr_pixel, uint16_t pixels_to_draw) {
        switch(range.type) {
            case NORMAL:
                return range.start_color;
            case GRADIENT:
                return ComputeGradientIntersect(curr_pixel, pixels_to_draw, range.start_color, range.end_color);
        }
    }

    void DrawRange(const PatternFrameRange& range, Pixel* hw_pixels, uint16_t hw_pixel_count) {
        uint16_t start_pixel = (hw_pixel_count * range.start) / RANGE_MAX;
        uint16_t end_pixel = (hw_pixel_count * range.end) / RANGE_MAX;
        uint16_t curr_pixel = 0;
        uint16_t pixels_to_draw = 0;
        if (start_pixel < end_pixel) {
            pixels_to_draw = end_pixel - start_pixel + 1;
        } else {
            pixels_to_draw = hw_pixel_count - start_pixel + end_pixel + 1;
        }

        if (start_pixel <= end_pixel){
            for (uint16_t p = start_pixel; p <= end_pixel; p++){
                hw_pixels[p] = ComputePixelColor(range, curr_pixel, pixels_to_draw);
                curr_pixel++;
            }
        } else {
            // wrap around to the start
            for (uint16_t p = start_pixel; p < hw_pixel_count; p++){
                hw_pixels[p] = ComputePixelColor(range, curr_pixel, pixels_to_draw);
                curr_pixel++;
            }
            for (uint16_t p = 0; p <= end_pixel; p++){
                hw_pixels[p] = ComputePixelColor(range, curr_pixel, pixels_to_draw);
                curr_pixel++;
            }
        }
    }

    void Transform(const PatternFrame* patternFrame, HwFrame* hwFrame){
        for (int i = 0; i < VERICAL_SHAPE_TRANSFORM_MAX_DIMS; i++) {
            Pixel* hw_pixels = hwFrame->getMutablePixels(i);
            uint16_t hw_pixel_count = hwFrame->getNumPixels(i);
            // Default is the pixel is off
            memset(hw_pixels, 0, hw_pixel_count*sizeof(Pixel));
            const PatternFrameDimension* curr_dimension = &patternFrame->dimensions[0];
            for (int r = 0; r < curr_dimension->rangeCount; r++) {
                DrawRange(curr_dimension->ranges[r], hw_pixels, hw_pixel_count);
            }
        }

    }
};

#endif