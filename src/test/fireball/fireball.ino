#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>

#define MAX_SPARKLES 15

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;

struct Sparkle {
   LinearRange* range;
   uint16_t lifespan;
};

uint16_t tail_length = RANGE_MAX / 4 / 2;
uint16_t ball_length = RANGE_MAX / 8 / 3;

class Fireball {
 public:
   Fireball() {}

   Fireball(LinearRange* ball_low, LinearRange* ball_high, LinearRange* flame,
            LinearRange* smoke)
       : ball_low_(ball_low), ball_high_(ball_high), flame_(flame),
         smoke_(smoke) {
      *smoke_ = LinearRange(0, 1 * tail_length / 4,
                            [](uint16_t curr, uint16_t total) -> Pixel {
                               switch (random(0, 2)) {
                               case 0:
                                  return YELLOW;
                               case 1:
                                  return RED;
                               default:
                                  return Pixel{0, 0, 0};
                               };
                            });
      *flame_ = LinearRange::Gradient(1 * tail_length / 4 + 1, tail_length,
                                      YELLOW, {0, 100, 255});
      uint16_t ball_start = tail_length + 1;
      *ball_low_ = LinearRange::Gradient(ball_start, ball_start + ball_length,
                                         BROWN, {255, 155, 0});
      *ball_high_ = LinearRange::Gradient(ball_start + ball_length + 1,
                                          ball_start + 2 * ball_length + 1,
                                          {255, 155, 0}, RED);
   }

   LinearRange* ball_low_;
   LinearRange* ball_high_;
   LinearRange* flame_;
   LinearRange* smoke_;

   void Slide(uint16_t increment) {
      ball_low_->Slide(increment);
      ball_high_->Slide(increment);
      flame_->Slide(increment);
      smoke_->Slide(increment);
   }

   void ResetPosition() {
      smoke_->SetPosition(0 - ball_length - tail_length);
      flame_->SetPosition(smoke_->end + 1);
      ball_low_->SetPosition(flame_->end + 1);
      ball_high_->SetPosition(ball_low_->end + 1);
   }
};

Sparkle sparkles[MAX_SPARKLES];
uint16_t new_sparkle;

Fireball fireball;
uint16_t fireball_distance;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);
   auto* dimension = pFrame.AddDimension();

   for (int i = 0; i < MAX_SPARKLES; i++) {
      sparkles[i].range = dimension->AddLayer();
      *(sparkles[i].range) = LinearRange::SolidColor(0, 0, YELLOW);
   }
   fireball_distance = random(2000, 4000);
   fireball = Fireball(dimension->AddLayer(), dimension->AddLayer(),
                       dimension->AddLayer(), dimension->AddLayer());
   fireball.ResetPosition();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();
}

Sparkle* FindSparkleSlot() {
   for (int i = 0; i < MAX_SPARKLES; i++) {
      if (sparkles[i].lifespan == 0) {
         return &sparkles[i];
      }
   }
   return nullptr;
}

#define DELAY 6UL
void loop() {
   unsigned long start_time = millis();

   // If the new sparkle countdown reached zero, try to create a new sparkle.
   if (new_sparkle == 0) {
      auto* sparkle = FindSparkleSlot();
      if (sparkle) {
         sparkle->range->SetPosition(random(0, RANGE_MAX));
         sparkle->range->SetAbsoluteLength(random(4, 8));
         sparkle->lifespan = random(60, 120);
         sparkle->range->SetVisible(true);
      }
      // Reset sparkle counter
      new_sparkle = random(10, 40);
   } else {
      new_sparkle--;
   }

   // For every existing sparkle, decrement its lifespan.
   for (int i = 0; i < MAX_SPARKLES; i++) {
      auto& sparkle = sparkles[i];
      if (sparkle.lifespan > 0) {
         sparkle.lifespan--;
      } else {
         // If the sparkle is dead, set invisible.
         sparkle.range->SetVisible(false);
      }
   }

   if (fireball.smoke_->start > fireball_distance) {
      // reset fireball
      fireball.ResetPosition();
      fireball_distance = random(2000, 4000);
   } else {
      fireball.Slide(5);
   }

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   if (render_time < DELAY) {
      delay(DELAY - render_time);
   }
}
