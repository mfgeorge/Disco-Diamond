#include "pattern.h"
#include "colors.h"
#include "color_util.h"

class TheaterChasePattern : public Pattern {
   public:
      TheaterChasePattern() : pos_(0), primary_color_(TURQUOISE) {}

      virtual void init(PatternFrame* pattern_frame) override {
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

    protected:
      LinearRange* range_;
      int pos_;

    private:
      Pixel primary_color_;
};

class RainbowTheaterChase : public TheaterChasePattern {
   public:
      RainbowTheaterChase() : TheaterChasePattern(), first_pixel_hue_(0) {}

      void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range_ = dimension->AddLayer();
         *range_ = LinearRange(
             0, RANGE_MAX, [this](uint16_t curr, uint16_t total) -> Pixel {
                uint16_t hue = first_pixel_hue_ + pos_ * 65535L / total;
                // Color every 3rd pixel, starting with pos
                return (curr % 3 == pos_) ? HSVToRGB(hue) : Pixel{0, 0, 0};
             });
         range_->EnableGammaCorrection();
      }

      void step() override {
         TheaterChasePattern::step();
         first_pixel_hue_ += 65536L/90;
      }

      private:
      uint16_t first_pixel_hue_;
};

class RastaTheaterChase : public TheaterChasePattern {
   public:
      RastaTheaterChase() : TheaterChasePattern(), color_idx(0), color_width(0) {
         colors[0] = GREEN;
         colors[1] = YELLOW;
         colors[2] = RED;
      }

      void init(PatternFrame* pattern_frame) override {
         auto dimension = pattern_frame->AddDimension();
         range_ = dimension->AddLayer();
         *range_ = LinearRange(
             0, RANGE_MAX, [this](uint16_t curr, uint16_t total) -> Pixel {
                if (curr % 3 == pos_) {
                   color_width = (color_width+1)%3;
                   if (color_width == 0) {
                      color_idx = (color_idx + 1) % 3;
                   }
                   return colors[color_idx];
                }
                return BLACK;
             });
         range_->EnableGammaCorrection();
      }

      void step() override {
         pos_--;
         if(pos_ < 0) {
            pos_ = 2;
         }
      }

      private:
      int color_idx;
      int color_width;
      Pixel colors[3];
};