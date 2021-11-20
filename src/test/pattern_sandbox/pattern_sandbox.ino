#include <Adafruit_NeoPixel.h>
#include <ledstrip.h>
#include <pattern.h>
#include <pins.h>
#include <shape_transform.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip[] = {
    Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2),
    Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};

HwFrame testHwFrame(strip, 4);

PatternFrame testPatternFrame;

VerticalShapeTransform testTransform;

uint16_t position = 0;
uint16_t length = 200;

void setup() {
   pinMode(PIN1, OUTPUT);
   pinMode(PIN2, OUTPUT);
   pinMode(PIN3, OUTPUT);
   pinMode(PIN4, OUTPUT);

   Serial.begin(115200);

   // initial settings for pattern frame
   testPatternFrame.activeDimensions = 1;
   testPatternFrame.dimensions[0].rangeCount = 2;

   // populate the pattern frame
   testPatternFrame.dimensions[0].ranges[0] =
       LinearRange::Gradient(0, 2 * length, {255, 0, 0}, {0, 255, 0});
   testPatternFrame.dimensions[0].ranges[1] =
       LinearRange::Gradient((RANGE_MAX / 2), (RANGE_MAX / 2) + (2 * length),
                             {0, 0, 255}, {0, 255, 0});

   testPatternFrame.dimensions[0].ranges[0].Wrap();
   testPatternFrame.dimensions[0].ranges[1].Wrap();
}

void loop() {
   unsigned long start_time = millis();

   length = uint16_t(50.0 * cos((float(position) * 6.7) / 3000.0) + 62.5) * 2;
   position += 5;

   testPatternFrame.dimensions[0].ranges[0].SetAbsoluteLength(length, TAIL);
   testPatternFrame.dimensions[0].ranges[1].SetAbsoluteLength(length, TAIL);
   testPatternFrame.dimensions[0].ranges[0].Slide(5);
   testPatternFrame.dimensions[0].ranges[1].Slide(5);

   testTransform.Transform(&testPatternFrame, &testHwFrame);
   testHwFrame.draw();

   unsigned long render_time = millis() - start_time;

   // Wait 10 ms between frames but render immediately if we are late.
   if (render_time < 10UL) {
      delay(10UL - render_time);
   }
}
