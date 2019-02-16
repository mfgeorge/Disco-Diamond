#include <ledstrip.h>
#include <pins.h>
#include <pattern.h>
#include <shape_transform.h>
#include <Adafruit_NeoPixel.h>


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

Adafruit_NeoPixel strip[] = { Adafruit_NeoPixel(42, PIN1), Adafruit_NeoPixel(42, PIN2), Adafruit_NeoPixel(42, PIN3), Adafruit_NeoPixel(42, PIN4)};
  
HwFrame testHwFrame(strip, 4);

PatternFrame testPatternFrame;

VerticalShapeTransform testTransform;

uint16_t i = 0;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);

  Serial.begin(115200);

  // initial settings for pattern frame 
  testPatternFrame.activeDimensions = 1;
  testPatternFrame.dimensions[0].rangeCount = 2;

  // populate the pattern frame 
  testPatternFrame.dimensions[0].ranges[0].start = 0;
  testPatternFrame.dimensions[0].ranges[0].end = 511;
  testPatternFrame.dimensions[0].ranges[0].pixel = {255, 0, 0};
  testPatternFrame.dimensions[0].ranges[1].start = 512;
  testPatternFrame.dimensions[0].ranges[1].end = 1023;
  testPatternFrame.dimensions[0].ranges[1].pixel = {0, 0, 255};
}

void loop() {
  Serial.println("-------------------------------------");

  testTransform.Transform(&testPatternFrame, &testHwFrame);

  testHwFrame.draw();


  testPatternFrame.dimensions[0].ranges[0].start = (100+i) % RANGE_MAX;
  testPatternFrame.dimensions[0].ranges[0].end = (311+i) % RANGE_MAX;
  testPatternFrame.dimensions[0].ranges[0].pixel = {255, 0, 0};
  testPatternFrame.dimensions[0].ranges[1].start = (712+i) % RANGE_MAX;
  testPatternFrame.dimensions[0].ranges[1].end = (923+i) % RANGE_MAX;
  testPatternFrame.dimensions[0].ranges[1].pixel = {0, 0, 255};

  i += 5;
}

