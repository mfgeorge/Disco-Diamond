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

LinearRange* range;
uint16_t hue = 0;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   // initial settings for pattern frame
   auto* dimension = pFrame.AddDimension();
   range = dimension->AddLayer();

   *range =
       LinearRange(0, RANGE_MAX, [](uint16_t curr, uint16_t total) -> Pixel {
          int h = hue + (curr * 65536L / total);
          return HSVToRGB(h);
       });
   range->EnableGammaCorrection();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();
}

#define DELAY 0UL
bool growing = true;
void loop() {
   unsigned long start_time = millis();
   hue = (hue + 256) % (5*65536);
   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   if (render_time < DELAY) {
      delay(DELAY - render_time);
   }
}
