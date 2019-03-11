#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "ledstrip.h"

#define RANGE_MAX 1024
#define MAX_RANGES 100
#define MAX_DIMENSIONS 10

enum PatternFrameType {
    NORMAL,
    GRADIENT
};

class AbstractPatternFrameRange {
    public:
    // 0 to 1024 normalized range
    uint16_t start;
    uint16_t end;
    PatternFrameType type;

    AbstractPatternFrameRange(PatternFrameType type) : type(type) {}

    AbstractPatternFrameRange(uint16_t start, uint16_t end, PatternFrameType type) 
        : start(start), end(end), type(type) {}
};

class PatternFrameRange : public AbstractPatternFrameRange {
    public:
    Pixel start_color;
    Pixel end_color;

    PatternFrameRange() : AbstractPatternFrameRange(NORMAL) {}

    PatternFrameRange(uint16_t start, uint16_t end, Pixel color) 
        : AbstractPatternFrameRange(start, end, NORMAL), 
          start_color(color) {}

    PatternFrameRange(uint16_t start, uint16_t end, Pixel start_color, Pixel end_color) 
        : AbstractPatternFrameRange(start, end, GRADIENT), 
          start_color(start_color), 
          end_color(end_color) {}
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