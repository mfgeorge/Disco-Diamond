#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "ledstrip.h"

#define RANGE_MAX 1024
#define MAX_RANGES 100
#define MAX_DIMENSIONS 10

struct PatternFrameRange {
    // 0 to 1024 normalized range
    uint16_t start; 
    uint16_t end; 
    Pixel pixel;
};

struct PatternFrameDimension {
    PatternFrameRange ranges[MAX_RANGES];
    uint8_t rangeCount;
};

struct PatternFrame {
    PatternFrameDimension dimensions[MAX_DIMENSIONS];
    uint8_t activeDimensions;
};

#endif