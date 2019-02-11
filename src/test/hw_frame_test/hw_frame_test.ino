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

  HwFrame testFrame(strip, 4);

  for (int i=0; i<4; i++){
    Pixel* testPixels = testFrame.getMutablePixels(i);
    uint16_t pixelCount = testFrame.getNumPixels(i);
    for (int j=0; j<pixelCount; j++){
      if (j<pixelCount/2-2) {
        testPixels[j] = {0, 0, 255};
      }
      else {
        testPixels[j] = {255, 0, 0};
      }
    }
  }

  testFrame.draw();

}

void loop() {

}

