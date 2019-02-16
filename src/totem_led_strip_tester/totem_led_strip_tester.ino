#include <dummy.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

#define OFF_BOARD_LED 13
#define OFF_BOARD_LED_PERIOD 2500000 // hardware timer1 has period of 200ns, so LED_PERIOD/5 = us
#define ON_BOARD_LED 2
#define ON_BOARD_LED_PERIOD 200 // software timer accepts time in ms 
#define NUM_STRIP_LEDS 16
#define LED_COUNTER 4

// software timer
Ticker blinker;

// led strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_STRIP_LEDS, OFF_BOARD_LED);
int currLED = 0;
int counter = 0;
int red = 0;
int blue = 0;
int green = 0;

// hardware interrupt for LED writing. 
void ICACHE_RAM_ATTR ledWriteISR(){
  // change colors
  green = green + 5;
  // change current LED
  counter++;
  if (counter >= LED_COUNTER) {
    strip.setPixelColor(currLED, 30, 0, 0);
    currLED++;
    if (currLED == NUM_STRIP_LEDS) {
      currLED = 0;
    }
    counter = 0;
  }
  strip.setPixelColor(currLED, red, green, blue);
  strip.show();
  timer1_write(OFF_BOARD_LED_PERIOD);
}

// software timing for rendering
void changeLEDState()
{
  digitalWrite(ON_BOARD_LED, !(digitalRead(ON_BOARD_LED)));  //Invert Current State of LED  
}
//=======================================================================
//                               Setup
//=======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("");

    // off board LED will use the hardware timer1 in this example
    pinMode(OFF_BOARD_LED,OUTPUT);
    timer1_attachInterrupt(ledWriteISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(ON_BOARD_LED_PERIOD);

    // on board LED will use the software timer in this example
    pinMode(ON_BOARD_LED,OUTPUT);
    blinker.attach_ms(ON_BOARD_LED_PERIOD, changeLEDState);

    // setup LED strip
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}
//=======================================================================
//                MAIN LOOP
//=======================================================================
void loop()
{
  /*
  green = green + 5;
  if (green >= 255) {
    green = 0;
  }
  currLED++;
  if (currLED >= NUM_STRIP_LEDS) {
    currLED = 0;
  }
  strip.setPixelColor(currLED, red, green, blue);
  strip.show();
  delay(250);
  */
}
//=======================================================================
