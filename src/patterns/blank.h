#include "pattern.h"
#include "colors.h"
#include "color_util.h"

class BlankPattern : public Pattern {
   public:
      BlankPattern() {}

      virtual void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range1_ = dimension->AddLayer();

         *range1_ = LinearRange::SolidColor(0, RANGE_MAX, BLACK);
      }

      void step() override { }

      uint16_t frame_delay() override {
         return 10UL;
      }

    private:
      LinearRange* range1_;
};