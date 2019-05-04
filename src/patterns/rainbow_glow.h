#include "pattern.h"

class RainbowGlowPattern : public Pattern {
   public:
      RainbowGlowPattern() : hue_(0) {}

      void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range_ = dimension->AddLayer();
         *range_ = LinearRange(0, RANGE_MAX,
                               [this](uint16_t curr, uint16_t total) -> Pixel {
                                  int h = hue_ + (curr * 65536L / total);
                                  return HSVToRGB(h);
                               });
      }

      void step() override {
         // Overflow at max uint16 on purpose.
         hue_ = hue_ + 256;
      }

      uint16_t frame_delay() {
         // Go as fast as possible.
         return 0;
      }


   private:
     uint16_t hue_;
     LinearRange* range_;
};