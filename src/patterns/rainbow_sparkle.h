#include "colors.h"
#include "pattern.h"

#define MAX_SPARKLES 15

struct Sparkle {
   LinearRange* range;
   uint16_t lifespan;
};

class RainbowSparklePattern : public Pattern {
 public:
   RainbowSparklePattern() {}

   void init(PatternFrame* pattern_frame) override {
      auto* dimension = pattern_frame->AddDimension();

      for (int i = 0; i < MAX_SPARKLES; i++) {
         sparkles[i].range = dimension->AddLayer();
         *(sparkles[i].range) = LinearRange(
             0, 0, [this](uint16_t start, uint16_t end) -> Pixel {
                int hue = random(0,65535);
                return HSVToRGB(hue);
             });
         //sparkles[i].range->EnableDithering();
         sparkles[i].range->EnableGammaCorrection();
      }
   }

   void step() override {
      // If the new sparkle countdown reached zero, try to create a new
      // sparkle.
      if (new_sparkle == 0) {
         auto* sparkle = FindSparkleSlot();
         if (sparkle) {
            sparkle->range->SetPosition(random(0, RANGE_MAX));
            sparkle->range->SetAbsoluteLength(random(4, 8));
            sparkle->lifespan = random(15, 30);
            sparkle->range->SetVisible(true);
         }
         // Reset sparkle counter
         new_sparkle = random(2, 5);
      } else {
         new_sparkle--;
      }

      // For every existing sparkle, decrement its lifespan.
      int16_t slide_amt;
      slide_amt = int16_t(69.0 * sin(0.69*cnt_));
      for (int i = 0; i < MAX_SPARKLES; i++) {
         auto& sparkle = sparkles[i];
         if (sparkle.lifespan > 0) {
            sparkle.lifespan--;
         } else {
            // If the sparkle is dead, set invisible.
            sparkle.range->SetVisible(false);
         }
         sparkle.range->Slide(slide_amt);
      }
      cnt_++;
   }

   uint16_t frame_delay() { return 50UL; }

   Sparkle* FindSparkleSlot() {
      for (int i = 0; i < MAX_SPARKLES; i++) {
         if (sparkles[i].lifespan == 0) {
            return &sparkles[i];
         }
      }
      return nullptr;
   }

 private:
   Sparkle sparkles[MAX_SPARKLES];
   uint16_t new_sparkle;
   uint16_t cnt_;
};