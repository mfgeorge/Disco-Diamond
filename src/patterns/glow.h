#include "pattern.h"

class GlowPattern : public Pattern {
 public:
   GlowPattern(uint16_t hue_low, uint16_t hue_high)
       : hue_low_(hue_low), hue_high_(hue_high) {}

   void init(PatternFrame* pattern_frame) override {
      auto dimension = pattern_frame->AddDimension();
      range_ = dimension->AddLayer();
      *range_ = LinearRange(
          0, RANGE_MAX, [this](uint16_t curr, uint16_t total) -> Pixel {
             uint16_t h;
             uint16_t diff = hue_high_ - hue_low_;
             uint16_t half = total / 2;

             
             if (curr < total / 2) {
                h = hue_low_ + (diff * curr / half) ;
             } else {
                h = hue_high_ -  ((curr - half) * diff / half);

             }
             return HSVToRGB(h);
          });
          range_->Wrap();
          range_->EnableGammaCorrection();
   }

   void step() override {
      // Overflow at max uint16 on purpose.
      range_->Slide(20);
   }

   uint16_t frame_delay() {
      // Go as fast as possible.
      return 30;
   }

 private:
   uint16_t hue_low_;
   uint16_t hue_high_;
   LinearRange* range_;
};