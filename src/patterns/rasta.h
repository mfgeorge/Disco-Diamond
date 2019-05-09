#include "pattern.h"
#include "colors.h"
#include "color_util.h"

class Rasta : public Pattern {
   public:
      Rasta() {}

      virtual void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range1_ = dimension->AddLayer();
         range2_ = dimension->AddLayer();
         range3_ = dimension->AddLayer();

         *range1_ = LinearRange(
             0, RANGE_MAX / 3,
             [this](uint16_t curr, uint16_t total) -> Pixel { return GREEN; });
         *range2_ = LinearRange(
             RANGE_MAX / 3, RANGE_MAX * 2 / 3,
             [this](uint16_t curr, uint16_t total) -> Pixel { return YELLOW; });
         *range3_ = LinearRange(
             RANGE_MAX * 2 / 3, RANGE_MAX,
             [this](uint16_t curr, uint16_t total) -> Pixel { return RED; });
         // range_->EnableGammaCorrection();
         range1_->Wrap();
         range2_->Wrap();
         range3_->Wrap();
      }

      void step() override {
         range1_->Slide(3);
         range2_->Slide(3);
         range3_->Slide(3);
      }

      uint16_t frame_delay() override {
         return 10UL;
      }

    private:
      LinearRange* range1_;
      LinearRange* range2_;
      LinearRange* range3_;
};