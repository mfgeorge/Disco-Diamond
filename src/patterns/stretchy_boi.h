#include "colors.h"
#include "pattern.h"
#include "color_util.h"
#include "color_picker.h"

class StretchyBoiPattern : public Pattern {
 public:
   // Two part version
   StretchyBoiPattern(Pixel color1, Pixel color2, bool color_switch=true)
       : growing(true), color1_(color1), color2_(color2), bg_color1_(WHITE),
         bg_color2_(PINK), color_switch_(color_switch) {}

   virtual void initRanges(uint16_t initial_length) {
      *lower_half = LinearRange(0, initial_length + 10,
                                [this](uint16_t, uint16_t) { return color1_; });
      *upper_half = LinearRange(initial_length, 2 * initial_length - 1,
                                [this](uint16_t, uint16_t) { return color2_; });
   }

   void init(PatternFrame* pattern_frame) override {
      auto* dimension = pattern_frame->AddDimension();
      background = dimension->AddLayer();
      lower_half = dimension->AddLayer();
      upper_half = dimension->AddLayer();

      uint16_t initial_length = 70;
      // Populate the pattern frame
      // Background stretches across the whole dimension.
      *background =
          LinearRange(0, RANGE_MAX, [this](uint16_t curr, uint16_t total) -> Pixel {
             // Alternate white and pink every 4 pixels.
             return (curr % 2) ? bg_color1_ : bg_color2_;
          });
      // Slightly overlap ranges
      initRanges(initial_length);

      if (color_switch_) {
         Square s = MakeSquare(RandomHue());
         setColors(s.colors[0], s.colors[2]);
         setBackground(s.colors[1], s.colors[3]);
      }

      lower_half->Wrap();
      upper_half->Wrap();
      lower_half->EnableGammaCorrection();
      upper_half->EnableGammaCorrection();
      background->EnableGammaCorrection();
   }

   void setColors(Pixel c1, Pixel c2) {
      color1_ = c1;
      color2_ = c2;
   }

   void setBackground(Pixel c1, Pixel c2) {
      bg_color1_ = DimColor(c1, 20);
      bg_color2_ = DimColor(c2, 20);
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

 protected:
   Pixel color1_;
   Pixel color2_;
   Pixel bg_color1_;
   Pixel bg_color2_;
   LinearRange* background;
   LinearRange* lower_half;
   LinearRange* upper_half;
   bool growing;
   bool color_switch_;
};

class GradientStretchyBoi : public StretchyBoiPattern {
 public:
   GradientStretchyBoi(Pixel color1, Pixel color2, Pixel color3)
       : StretchyBoiPattern(color1, color2, false), color3_(color3) {}

   void initRanges(uint16_t initial_length) override {
      *lower_half =
          LinearRange::Gradient(0, initial_length + 10, color1_, color2_);
      *upper_half = LinearRange::Gradient(
          initial_length, 2 * initial_length - 1, color2_, color3_);
   }

 private:
   Pixel color3_;
};