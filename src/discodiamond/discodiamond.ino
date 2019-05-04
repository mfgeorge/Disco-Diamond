#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>
#include <patterns/rainbow_glow.h>

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;

RainbowGlowPattern pattern;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   pattern.init(&pFrame);
}

void loop() {
   unsigned long start_time = millis();
   pattern.step();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   unsigned long frame_delay = pattern.frame_delay(); 
   if (render_time < frame_delay) {
      delay(frame_delay - render_time);
   }
}
