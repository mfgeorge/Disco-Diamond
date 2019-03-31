#include <Adafruit_NeoPixel.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>

Adafruit_NeoPixel strip[] = {Adafruit_NeoPixel(144, PIN1)};
HwFrame hwFrame(strip, 1);
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
   strip[0].setBrightness(100);

   // initial settings for pattern frame
   auto* dimension = pFrame.AddDimension();
   background = dimension->AddLayer();
   lower_half = dimension->AddLayer();
   upper_half = dimension->AddLayer();

   uint16_t initial_length = 200;
   // Populate the pattern frame
   // Background stretches across the whole dimension.
   *background =
       LinearRange(0, RANGE_MAX, [](uint16_t curr, uint16_t total) -> Pixel {
          // Alternate white and pink every 4 pixels.
          return ((curr / 4) % 2) ? Pixel{50, 0, 42} : WHITE;
       });
   *lower_half =
       LinearRange::Gradient(0, initial_length - 1, RED, GREEN);
   *upper_half = LinearRange::Gradient(initial_length, 2 * initial_length - 1,
                                       GREEN, BLUE);

   lower_half->Wrap();
   upper_half->Wrap();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();
}

#define DELAY 12UL
bool growing = true;
void loop() {
   unsigned long start_time = millis();

   lower_half->Slide(5);
   upper_half->Slide(5);

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
