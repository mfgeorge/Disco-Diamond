#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>
#include <patterns/rainbow_glow.h>
#include <patterns/theater_chase.h>
#include <patterns/stretchy_boi.h>
#include <patterns/rainbow_sparkle.h>
#include <patterns/pong.h>
#include <patterns/glow.h>
#include <patterns/slide_expand.h>

#define PATTERN_DURATION_SEC 10

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;

RainbowGlowPattern rainbow_glow;
TheaterChasePattern theater_chase;
StretchyBoiPattern stretchy_boi(RED, GREEN, BLUE);
StretchyBoiPattern stretchy_boi2(TURQUOISE, PINK);
RainbowSparklePattern rainbow_sparkle;
PongPattern pong(4, 8, 20, 2, 0, RANGE_MAX);
StretchyBoiPattern stretchy_boi3(BLACK, BLACK);
GlowPattern glow1(HueInDegrees(180), HueInDegrees(300));
SlideExpandPattern slide_expand;
RainbowTheaterChase rainbow_chase;

class PatternSwitcher {
 public:
   PatternSwitcher()
       : pattern_start_time_(millis()), pattern_index_(-1), total_patterns_(0) {}

   void AddPattern(Pattern* pattern) {
      patterns[total_patterns_] = pattern;
      total_patterns_ += 1;
   }

   inline Pattern* CurrPattern() {
      return curr_pattern_;
   }

   inline unsigned long PatternStartTime() {
      return pattern_start_time_;
   }
   
   void NextPattern() {
      pFrame.ClearDimensions();
      pattern_index_ = (pattern_index_ + 1) % total_patterns_;
      curr_pattern_ = patterns[pattern_index_];
      pattern_start_time_ = millis();
      curr_pattern_->init(&pFrame);
   }

   void PrevPattern() {
      pFrame.ClearDimensions();
      if(pattern_index_ == 0) {
         pattern_index_ = total_patterns_ - 1;
      } else {
         pattern_index_--;
      }
      curr_pattern_ = patterns[pattern_index_];
      pattern_start_time_ = millis();
      curr_pattern_->init(&pFrame);
   }

   private:
      Pattern* patterns[20];
      unsigned long pattern_start_time_;
      int pattern_index_;
      int total_patterns_;
      Pattern* curr_pattern_;
};

PatternSwitcher pattern_switcher;

void nextPattern(){
   noInterrupts();
   pattern_switcher.NextPattern();
   interrupts();
}

void prevPattern(){
   noInterrupts();
   pattern_switcher.PrevPattern();
   interrupts();
}

void setup() {
   ESP.wdtDisable();

   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);
   pinMode(BUTTON_1, INPUT_PULLUP);
   pinMode(BUTTON_2, INPUT_PULLUP);

   attachInterrupt(digitalPinToInterrupt(BUTTON_1), nextPattern, FALLING);
   attachInterrupt(digitalPinToInterrupt(BUTTON_2), prevPattern, FALLING);

   Serial.begin(115200);

   pattern_switcher.AddPattern(&rainbow_glow);
   pattern_switcher.AddPattern(&theater_chase);
   pattern_switcher.AddPattern(&stretchy_boi);
   pattern_switcher.AddPattern(&rainbow_sparkle);
   pattern_switcher.AddPattern(&stretchy_boi2);
   pattern_switcher.AddPattern(&pong);
   pattern_switcher.AddPattern(&stretchy_boi3);
   pattern_switcher.AddPattern(&glow1);
   pattern_switcher.AddPattern(&slide_expand);
   pattern_switcher.AddPattern(&rainbow_chase);
   pattern_switcher.NextPattern();
}

void draw() {
   unsigned long start_time = millis();
   pattern_switcher.CurrPattern()->step();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   unsigned long frame_delay = pattern_switcher.CurrPattern()->frame_delay(); 
   if (render_time < frame_delay) {
      delay(frame_delay - render_time);
   }
}

void loop() {
   unsigned long now = millis();
   // Serial.printf("now %d, start time %d\r\n", now, pattern_switcher.PatternStartTime());
   // if (now > pattern_switcher.PatternStartTime() + PATTERN_DURATION_SEC * 1000) {
   //    pattern_switcher.NextPattern();
   // }
   if (Serial.available() > 0) {
      auto c = Serial.read();
      if (c == 'n') {
         pattern_switcher.NextPattern();
      } else if (c == 'b') {
         pattern_switcher.PrevPattern();
      }
   }

   draw();

}

