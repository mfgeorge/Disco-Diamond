#include "colors.h"
#include "pattern.h"

class StretchyBoiPattern : public Pattern {
 public:
   // Two part version
   StretchyBoiPattern(Pixel color1, Pixel color2)  : gradient(false), growing(true), color1_(color1), color2_(color2) {}
   // Gradient version
   StretchyBoiPattern(Pixel color1, Pixel color2, Pixel color3)  : gradient(true), growing(true),  color1_(color1), color2_(color2), color3_(color3) {}

   void init(PatternFrame* pattern_frame) override {
      auto* dimension = pattern_frame->AddDimension();
      background = dimension->AddLayer();
      lower_half = dimension->AddLayer();
      upper_half = dimension->AddLayer();

      uint16_t initial_length = 70;
      // Populate the pattern frame
      // Background stretches across the whole dimension.
      *background =
          LinearRange(0, RANGE_MAX, [](uint16_t curr, uint16_t total) -> Pixel {
             // Alternate white and pink every 4 pixels.
             return (curr % 2) ? Pixel{100, 0, 66} : Pixel{50, 50, 50};
          });
      // Slightly overlap ranges
      if (gradient) {
         *lower_half =
             LinearRange::Gradient(0, initial_length + 10, color1_, color2_);
         *upper_half = LinearRange::Gradient(
             initial_length, 2 * initial_length - 1, color2_, color3_);
      } else {
         *lower_half =
             LinearRange::SolidColor(0, initial_length + 10, color1_);
         *upper_half = LinearRange::SolidColor(initial_length,
                                               2 * initial_length - 1, color2_);
      }

      lower_half->Wrap();
      upper_half->Wrap();
      lower_half->EnableGammaCorrection();
      upper_half->EnableGammaCorrection();
      background->EnableGammaCorrection();
   }

   void step() override {
      lower_half->Slide(3);
      upper_half->Slide(3);

      uint16_t length = lower_half->Length();
      if (length > 420) {
         growing = false;
      }
      if (length < 100) {
         growing = true;
      }
      if (growing) {
         lower_half->Stretch(1.07, TAIL);
         upper_half->Stretch(1.07, FRONT);
      } else {
         lower_half->Stretch(0.93, TAIL);
         upper_half->Stretch(0.93, FRONT);
      }
   }

   uint16_t frame_delay() override { return 12UL; }

 private:
   Pixel color1_;
   Pixel color2_;
   Pixel color3_;
   bool gradient;
   LinearRange* background;
   LinearRange* lower_half;
   LinearRange* upper_half;
   bool growing;
};