#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;

LinearRange* background;
LinearRange* lower_half;
LinearRange* upper_half;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   // initial settings for pattern frame
   auto* dimension = pFrame.AddDimension();
   background = dimension->AddLayer();
   lower_half = dimension->AddLayer();
   upper_half = dimension->AddLayer();

   background->EnableGammaCorrection();
   lower_half->EnableGammaCorrection();
   upper_half->EnableGammaCorrection();

   uint16_t initial_length = 70;
   // Populate the pattern frame
   // Background stretches across the whole dimension.
   *background =
       LinearRange(0, RANGE_MAX, [](uint16_t curr, uint16_t total) -> Pixel {
          // Alternate white and pink every 4 pixels.
          return (curr % 2) ? Pixel{100, 0, 66} : Pixel{50,50,50};
       });
   // Slightly overlap ranges
   *lower_half = LinearRange::Gradient(0, initial_length + 10, RED, GREEN);
   *upper_half = LinearRange::Gradient(initial_length, 2 * initial_length - 1,
                                       GREEN, BLUE);

   lower_half->Wrap();
   upper_half->Wrap();
   lower_half->EnableDithering();
   upper_half->EnableDithering();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();
}

#define DELAY 12UL
bool growing = true;
void loop() {
   unsigned long start_time = millis();

   lower_half->Slide(3);
   upper_half->Slide(3);

   uint16_t length = lower_half->Length();
   if (length > 420) {
      growing = false;
   }
   if (length < 100) {
      growing = true;
   }
   if (growing) {
      lower_half->Stretch(1.07, TAIL);
      upper_half->Stretch(1.07, FRONT);
   } else {
      lower_half->Stretch(0.93, TAIL);
      upper_half->Stretch(0.93, FRONT);
   }

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   if (render_time < DELAY) {
      delay(DELAY - render_time);
   }
}
