#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "ledstrip.h"

#define RANGE_MAX 1024
#define MAX_RANGES 100
#define MAX_DIMENSIONS 10

enum RangeType {
    NORMAL,
    GRADIENT
};

enum StretchType {
    FRONT,
    TAIL,
    CENTER,
};

class AbstractLinearRange {
    public:
    // 0 to 1024 normalized range
    uint16_t start;
    uint16_t end;
    RangeType type;

    // Indicates whether or not this should be wrapped across the dimension if it extends past the end.
    bool wrapped;

    AbstractLinearRange(RangeType type) : type(type), wrapped(false) {}

    AbstractLinearRange(uint16_t start, uint16_t end, RangeType type) 
        : start(start), end(end), type(type), wrapped(false) {}

    void Slide(uint16_t increment) {
        uint16_t length = end - start;
        start = (start + increment) % RANGE_MAX;
        end = start + length;
    }

    void Wrap() {
        wrapped = true;
    }

    bool IsWrapped() const {
        return wrapped;
    }

    void SetAbsoluteLength(uint16_t length) {
        end = start + length;
    }

    void Stretch(float stretch_factor, StretchType stretch_type = TAIL) {
        uint16_t length = end - start;
        uint16_t stretched_length = length * stretch_factor;
        switch (stretch_type) {
            case TAIL:
                end = start + stretched_length;
            case FRONT:
                start = end - stretched_length;
            case CENTER:
                uint16_t center = start + (length / 2);
                start = center - (stretched_length / 2);
                end = center + (stretched_length / 2);
        }
    }
};

class LinearRange : public AbstractLinearRange {
    public:
    Pixel start_color;
    Pixel end_color;

    LinearRange() : AbstractLinearRange(NORMAL) {}

    LinearRange(uint16_t start, uint16_t end, Pixel color) 
        : AbstractLinearRange(start, end, NORMAL), 
          start_color(color) {}

    LinearRange(uint16_t start, uint16_t end, Pixel start_color, Pixel end_color) 
        : AbstractLinearRange(start, end, GRADIENT), 
          start_color(start_color), 
          end_color(end_color) {}
};

struct PatternFrameDimension {
    LinearRange ranges[MAX_RANGES];
    uint8_t rangeCount;
};

struct PatternFrame {
    PatternFrameDimension dimensions[MAX_DIMENSIONS];
    uint8_t activeDimensions;
};

#endif