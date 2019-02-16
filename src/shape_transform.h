#ifndef SHAPE_TRANSFORM_H
#define SHAPE_TRANSFORM_H

#include "Arduino.h"
#include "ledstrip.h"
#include "pattern.h"

#define VERICAL_SHAPE_TRANSFORM_MAX_DIMS 4

class VerticalShapeTransform {
public:
    void Transform(const PatternFrame* patternFrame, HwFrame* hwFrame){
        for (int i = 0; i < VERICAL_SHAPE_TRANSFORM_MAX_DIMS; i++) {
            Pixel* hw_pixels = hwFrame->getMutablePixels(i);
            uint16_t hw_pixel_count = hwFrame->getNumPixels(i);
            // Default is the pixel is off
            memset(hw_pixels, 0, hw_pixel_count*sizeof(Pixel));
            const PatternFrameDimension* curr_dimension = &patternFrame->dimensions[0];
            for (int r = 0; r < curr_dimension->rangeCount; r++) {
                const PatternFrameRange* range = &curr_dimension->ranges[r];
                uint16_t start_pixel = (hw_pixel_count * range->start) / RANGE_MAX;
                uint16_t end_pixel = (hw_pixel_count * range->end) / RANGE_MAX;
                if (start_pixel < end_pixel){
                    for (uint16_t p = start_pixel; p <= end_pixel; p++){
                        hw_pixels[p] = range->pixel; 
                    }
                }
                else {
                    // wrap around to the start
                    for (uint16_t p = start_pixel; p < hw_pixel_count; p++){
                        hw_pixels[p] = range->pixel; 
                    }
                    for (uint16_t p = 0; p <= end_pixel; p++){
                        hw_pixels[p] = range->pixel; 
                    }
                }
            }
        }

    }
};

#endif