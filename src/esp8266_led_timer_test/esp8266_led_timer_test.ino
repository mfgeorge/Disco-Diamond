#include <dummy.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

/*
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
*/

#define OFF_BOARD_LED 13
#define OFF_BOARD_LED_PERIOD 2500000 // hardware timer1 has period of 200ns, so LED_PERIOD/5 = us
#define ON_BOARD_LED 2
#define ON_BOARD_LED_PERIOD 200 // software timer accepts time in ms 

Ticker blinker;

//=======================================================================
void ICACHE_RAM_ATTR onTimerISR(){
    digitalWrite(OFF_BOARD_LED,!(digitalRead(OFF_BOARD_LED)));  //Toggle LED Pin
    timer1_write(OFF_BOARD_LED_PERIOD);
}

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
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(ON_BOARD_LED_PERIOD);

    // on board LED will use the software timer in this example
    pinMode(ON_BOARD_LED,OUTPUT);
    blinker.attach_ms(ON_BOARD_LED_PERIOD, changeLEDState);
}
//=======================================================================
//                MAIN LOOP
//=======================================================================
void loop()
{
}
//=======================================================================
