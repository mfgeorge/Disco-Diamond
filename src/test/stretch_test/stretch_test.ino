#include <Adafruit_NeoPixel.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>

Adafruit_NeoPixel strip[] = {Adafruit_NeoPixel(144, PIN1)};
HwFrame hwFrame(strip, 1);
PatternFrame pFrame;
VerticalShapeTransform transform;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   // initial settings for pattern frame
   pFrame.activeDimensions = 1;
   pFrame.dimensions[0].rangeCount = 2;

   uint16_t length = 200;
   // populate the pattern frame

   pFrame.dimensions[0].ranges[0] = LinearRange::Gradient(
       RANGE_MAX / 2 - length, RANGE_MAX / 2, {50, 0, 0}, {0, 50, 0});

   pFrame.dimensions[0].ranges[1] = LinearRange::Gradient(
       RANGE_MAX / 2 + 1, RANGE_MAX / 2 + length, {0, 50, 0}, {0, 0, 50});

   pFrame.dimensions[0].ranges[0].Wrap();
   pFrame.dimensions[0].ranges[1].Wrap();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();
}

void print() {
   Serial.print("f1 pos ");
   Serial.print(pFrame.dimensions[0].ranges[0].start);
   Serial.print(" ");
   Serial.println(pFrame.dimensions[0].ranges[0].end);

   Serial.print("f2 pos ");
   Serial.print(pFrame.dimensions[0].ranges[1].start);
   Serial.print(" ");
   Serial.println(pFrame.dimensions[0].ranges[1].end);
}

bool growing = true;
void loop() {
   unsigned long start_time = millis();

   uint16_t length = pFrame.dimensions[0].ranges[0].Length();
   if (length > 300) {
      growing = false;
   }
   if (length < 100) {
      growing = true;
   }

   for (int i = 0; i < 2; i++) {
      pFrame.dimensions[0].ranges[i].Slide(5);
   }

   if (growing) {
      pFrame.dimensions[0].ranges[0].Stretch(1.1, TAIL);
      pFrame.dimensions[0].ranges[1].Stretch(1.1, FRONT);
   } else {
      pFrame.dimensions[0].ranges[0].Stretch(0.9, TAIL);
      pFrame.dimensions[0].ranges[1].Stretch(0.9, FRONT);
   }

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   if (render_time < 12UL) {
      delay(12UL - render_time);
   }
}
