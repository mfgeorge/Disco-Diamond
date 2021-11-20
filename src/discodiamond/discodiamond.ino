#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>
#include <color_picker.h>
#include <patterns/rainbow_glow.h>
#include <patterns/theater_chase.h>
#include <patterns/stretchy_boi.h>
#include <patterns/rainbow_sparkle.h>
#include <patterns/pong.h>
#include <patterns/glow.h>
#include <patterns/slide_expand.h>
#include <patterns/rasta.h>
#include <patterns/blank.h>

#define PATTERN_DURATION_SEC 10

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;

RainbowGlowPattern rainbow_glow;
TheaterChasePattern theater_chase;
GradientStretchyBoi rainbow_stretchy_boi(RED, GREEN, BLUE);

StretchyBoiPattern stretchy_boi_dark(BLACK, BLACK, false);
StretchyBoiPattern stretchy_boi_color(TURQUOISE, PINK);

RainbowSparklePattern rainbow_sparkle;


RainbowPongPattern rainbow_pongos(4, 8, 26, 2, 0, RANGE_MAX);
PongPattern pong(4, 8, 26, 2, 0, RANGE_MAX);

GlowPattern glow1(HueInDegrees(180), HueInDegrees(300));

RastaTheaterChase rasta_chase;

BlankPattern blank;

SlideExpandPattern slide_expand;
RainbowTheaterChase rainbow_chase;

Rasta rasta;

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

long last_button_time = 0;
#define BUTTON_DEBOUNCE_WINDOW_MS 100

void nextPattern(){
   long now = millis();
   if (now < last_button_time + BUTTON_DEBOUNCE_WINDOW_MS) {
      return;
   }
   noInterrupts();
   last_button_time = now;
   pattern_switcher.NextPattern();
   interrupts();
}

void prevPattern(){
   long now = millis();
   if (now < last_button_time + BUTTON_DEBOUNCE_WINDOW_MS) {
      return;
   }
   noInterrupts();
   last_button_time = now;
   pattern_switcher.PrevPattern();
   interrupts();
}

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);
   pinMode(BUTTON_1, INPUT_PULLUP);
   pinMode(BUTTON_2, INPUT_PULLUP);

   attachInterrupt(digitalPinToInterrupt(BUTTON_1), nextPattern, FALLING);
   attachInterrupt(digitalPinToInterrupt(BUTTON_2), prevPattern, FALLING);

   Serial.begin(115200);

   pattern_switcher.AddPattern(&rainbow_sparkle);
   pattern_switcher.AddPattern(&stretchy_boi_dark);
   pattern_switcher.AddPattern(&stretchy_boi_color);
   pattern_switcher.AddPattern(&rainbow_pongos);
   pattern_switcher.AddPattern(&pong);
   pattern_switcher.AddPattern(&glow1);
   pattern_switcher.AddPattern(&slide_expand);
   pattern_switcher.AddPattern(&rainbow_chase);
   pattern_switcher.AddPattern(&rasta);
   pattern_switcher.AddPattern(&rasta_chase);
   pattern_switcher.AddPattern(&rainbow_glow);
   pattern_switcher.AddPattern(&rainbow_stretchy_boi);
   pattern_switcher.AddPattern(&theater_chase);
   pattern_switcher.AddPattern(&blank);
   pattern_switcher.NextPattern();

   rainbow_stretchy_boi.setBackground(WHITE, PINK);
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

uint16_t wdt = 0;

void loop() {
   // Feed the watchdog every 500 frames
   wdt = (wdt + 1) % 500;
   if (wdt == 0) {
      ESP.wdtFeed();
   }

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

