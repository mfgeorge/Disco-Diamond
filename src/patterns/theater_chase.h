#include "pattern.h"
#include "colors.h"

class TheaterChasePattern : public Pattern {
   public:
      TheaterChasePattern() : pos_(0), primary_color_(TURQUOISE) {}

      void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range_ = dimension->AddLayer();
         *range_ = LinearRange(
             0, RANGE_MAX, [this](uint16_t curr, uint16_t total) -> Pixel {
                // Color every 3rd pixel, starting with pos
                return (curr % 3 == pos_) ? primary_color_ : Pixel{0, 0, 0};
             });
         range_->EnableGammaCorrection();
      }

      void step() override {
         pos_ = (pos_ + 1) % 3;
      }

      uint16_t frame_delay() override {
         return 50UL;
      }

   private:
     uint16_t pos_;
     Pixel primary_color_;
     LinearRange* range_;
};