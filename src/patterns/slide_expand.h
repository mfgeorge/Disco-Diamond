#include "pattern.h"

class SlideExpandPattern : public Pattern {
 public:
   SlideExpandPattern()
       : position_(0), length_(200) {}

   void init(PatternFrame* pattern_frame) override {

      auto dimension = pattern_frame->AddDimension();
      range1_ = dimension->AddLayer();
      range2_ = dimension->AddLayer();

      *range1_ = LinearRange::Gradient(0, 2 * length_, {255, 0, 0}, {0, 255, 0});
      *range2_ = LinearRange::Gradient((RANGE_MAX / 2), (RANGE_MAX / 2) + (2 * length_),
                             {0, 0, 255}, {0, 255, 0});
          range1_->Wrap();
          range1_->EnableGammaCorrection();
          range1_->EnableDithering();
          range2_->Wrap();
          range2_->EnableDithering();
          range2_->EnableGammaCorrection();
   }

   void step() override {
      length_ =
          uint16_t(50.0 * cos((float(position_) * 6.7) / 3000.0) + 62.5) * 2;
      position_ += 5;

      range1_->SetAbsoluteLength(length_, TAIL);
      range2_->SetAbsoluteLength(length_, TAIL);
      range1_->Slide(5);
      range2_->Slide(5);
   }

   uint16_t frame_delay() {
      return 10;
   }

 private:
   uint16_t position_;
   uint16_t length_;
   LinearRange* range1_;
   LinearRange* range2_;
};