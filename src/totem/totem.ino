#include <dummy.h>
#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

#define HARDWARE_WRITE_PERIOD 83333 // hardware timer1 has period of 200ns, so LED_PERIOD/5 = us
#define RENDER_PERIOD 17 // software timer accepts time in ms 
#define NUM_PIXELS 42 // all strips have 42 pixels for now

// GPIO pin mapping from MCU to header pins is defined on this schematic:
// https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF
// GPIO16 = D0
// GPIO5 = D1
// GPIO4 = D2
// GPIO0 = D3
#define D0 16
#define D1 5
#define D2 4
#define D3 0

//=============================================================================================
// Hardware Layer
//=============================================================================================
struct pixel{
    int red;                // 0 to 255 
    int blue;               // 0 to 255
    int green;              // 0 to 255
};

struct ledStrip{
    //int radialPos;          // 0 to 360 degrees
    pixel pixels[100];       // array of pixels, max size 100 for now
    int numPixels;
};

struct shapedFrame{
    // GPIO pin mapping from MCU to header pins is defined on this schematic:
    // https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF
    // GPIO16 = D0
    // GPIO5 = D1
    // GPIO4 = D2
    // GPIO0 = D3
    ledStrip strips[4];
    int numStrips = 4;
    bool readyForOutput;
};

//=============================================================================================
// Globals
//=============================================================================================
shapedFrame frame0;
Adafruit_NeoPixel strips[4];

//=============================================================================================
// Hardware Functions
//=============================================================================================

void updateStripColors(Adafruit_NeoPixel* strip, ledStrip* ledStrip){
    for(int i=0; i<ledStrip->numPixels; i++){
        int red = ledStrip->pixels[i].red;
        int green = ledStrip->pixels[i].green;
        int blue = ledStrip->pixels[i].blue;
        strip->setPixelColor(i, red, green, blue);
    }
    strip->show();
}

void clearStrip(ledStrip* ledStrip){
    for(int j=0; j<ledStrip->numPixels; j++){
        ledStrip->pixels[j].red = 0;
        ledStrip->pixels[j].green = 255;
        ledStrip->pixels[j].blue = 0;
    }
}

void readShapedFrame(shapedFrame* frame){
    for(int i=0; i<frame->numStrips; i++){
        updateStripColors(&strips[i], &frame->strips[i]);
    }
}

//=============================================================================================
// Rendering Functions
//=============================================================================================
void write_pixels(int start_pixel, int end_pixel, ledStrip* ledStrip, pixel pixel){
    for(int i=start_pixel; i<end_pixel; i++){
        ledStrip->pixels[i].red = pixel.red;
        ledStrip->pixels[i].green = pixel.green;
        ledStrip->pixels[i].blue = pixel.blue;
    }
}


//=============================================================================================
// Software Timers
//=============================================================================================
Ticker softwareTimer1;

// rendering ISR
void renderingISR(){
}

//=============================================================================================
// Hardware Timer ISR
//=============================================================================================
// hardware interrupt for LED writing
void ICACHE_RAM_ATTR ledWriteISR(){
    readShapedFrame(&frame0);
}

//=============================================================================================
// Setup
//=============================================================================================
void setup(){
    // serial communication
    Serial.begin(115200);
    Serial.println("");

    // instantiate neopixel objects
    strips[0] = Adafruit_NeoPixel(NUM_PIXELS, D0);
    strips[1] = Adafruit_NeoPixel(NUM_PIXELS, D1);
    strips[2] = Adafruit_NeoPixel(NUM_PIXELS, D2);
    strips[3] = Adafruit_NeoPixel(NUM_PIXELS, D3);

    // initialize shapedFrame objects
    frame0.readyForOutput = false;
    for(int i=0; i<frame0.numStrips; i++){
        frame0.strips[i].numPixels = NUM_PIXELS;
        clearStrip(&frame0.strips[i]);
    }
    
    // off board LED will use the hardware timer1 in this example
    timer1_attachInterrupt(ledWriteISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
    timer1_write(HARDWARE_WRITE_PERIOD);

    // on board LED will use the software timer in this example
    softwareTimer1.attach_ms(RENDER_PERIOD, renderingISR);

    // initialize shapedFrame objects
    pixel greenPixel = {0, 255, 0};
    for(int i=0; i<frame0.numStrips; i++){
        write_pixels(0, NUM_PIXELS, &frame0.strips[i], greenPixel);
    }
}
//=======================================================================
// Main Loop
//=======================================================================
void loop(){
    Serial.println(frame0.strips[0].pixels[0].green);
    delay(1000);
}
//=======================================================================
