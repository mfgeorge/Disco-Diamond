#include "pattern.h"
#include "colors.h"
#include "color_util.h"

class Rasta : public Pattern {
   public:
      Rasta() {}

      virtual void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range1_ = dimension->AddLayer();

         *range1_ = LinearRange(0, RANGE_MAX,
                                [this](uint16_t curr, uint16_t total) -> Pixel {
                                   if (curr < total / 3) {
                                      return GREEN;
                                   }
                                   if (curr < total * 2 / 3) {
                                      return YELLOW;
                                   }
                                   return RED;
                                });
         range1_->Wrap();
      }

      void step() override {
         range1_->Slide(3);
      }

      uint16_t frame_delay() override {
         return 10UL;
      }

    private:
      LinearRange* range1_;
};