#ifndef PATTERN_H
#define PATTERN_H

#include "Arduino.h"
#include "ledstrip.h"

#define RANGE_MAX 1023
#define MAX_RANGES 100
#define MAX_DIMENSIONS 10

enum RangeType {
    NORMAL,
    GRADIENT
};

enum AnchorType {
    FRONT,
    TAIL,
    CENTER,
};

class AbstractLinearRange {
   public:
    int16_t start;
    int16_t end;
    RangeType type;

    // Indicates whether or not this should be wrapped across the dimension if it extends past the end.
    bool wrapped;

    AbstractLinearRange(RangeType type) : type(type), wrapped(false) {}

    AbstractLinearRange(int16_t start, int16_t end, RangeType type) 
        : start(start), end(end), type(type), wrapped(false) {}

    void Wrap() {
        wrapped = true;
    }

    bool IsWrapped() const {
        return wrapped;
    }

    int16_t Center() const {
        return (end - start) / 2;
    }

    uint16_t Length() const {
        return end - start + 1;
    }

    void Slide(int16_t increment) {
        int16_t length = Length();
        start = (start + increment) % RANGE_MAX;
        end = start + length - 1;
    }

    void SetAbsoluteLength(uint16_t length, AnchorType anchor_type = FRONT) {
        switch(anchor_type) {
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
};

class LinearRange : public AbstractLinearRange {
    public:
    Pixel start_color;
    Pixel end_color;

    LinearRange() : AbstractLinearRange(NORMAL) {}

    LinearRange(int16_t start, int16_t end, Pixel color) 
        : AbstractLinearRange(start, end, NORMAL), 
          start_color(color) {}

    LinearRange(int16_t start, int16_t end, Pixel start_color, Pixel end_color) 
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