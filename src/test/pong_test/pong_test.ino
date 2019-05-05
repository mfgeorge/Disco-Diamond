#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>
#include <patterns/pong.h>

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
HwFrame hwFrame(strip, 4);
PatternFrame pFrame;
VerticalShapeTransform transform;


PongPattern pattern1(2, 4, 11, 0, 0, RANGE_MAX/2);
PongPattern pattern2(2, 4, 11, 2, RANGE_MAX/2, RANGE_MAX);

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   pattern1.init(&pFrame);
   pattern2.init(&pFrame);
}  

void loop() {
   unsigned long start_time = millis();
   pattern1.step();
   pattern2.step();

   transform.Transform(&pFrame, &hwFrame);
   hwFrame.draw();

   unsigned long render_time = millis() - start_time;
   unsigned long frame_delay = pattern1.frame_delay(); 
   if (render_time < frame_delay) {
      delay(frame_delay - render_time);
   }
}
