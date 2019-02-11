#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

#define MAX_SIZE 100

// code info https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
// these are the led's we used for the snowglobe 2019 totem pole: https://www.adafruit.com/product/1461

struct Pixel {
    uint8_t red;                // 0 to 255 
    uint8_t green;              // 0 to 255
    uint8_t blue;               // 0 to 255
};

struct HwFrameComponent {
    Adafruit_NeoPixel* strip;
    Pixel pixels[MAX_SIZE];
};

class HwFrame {

    public : 
        // GPIO pin mapping from MCU to header pins is defined on this schematic:
        // https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF
        // GPIO16 = D0
        // GPIO5 = D1
        // GPIO4 = D2
        // GPIO0 = D3
        HwFrame(Adafruit_NeoPixel* strips, int stripCount) {
            for (int i = 0; i < stripCount; i++) {
                components_[i].strip = strips + i;
            }
            stripCount_ = stripCount;
        }

        void draw() {
            for (int i = 0; i < stripCount_; i++) {
                showStrip(&components_[i]);
            }
        }

        bool isWriting(){
            return writing_;
        }

        void startWriting() {
            writing_ = true;
        }

        void finishWriting() {
            writing_ = false;
        }

        int getStripCount(){
            return stripCount_;
        }

        uint16_t getNumPixels(int stripIndex){
            return components_[stripIndex].strip->numPixels();
        }

        Pixel* getMutablePixels(int stripIndex){
            return components_[stripIndex].pixels;
        }

    private : 
        HwFrameComponent components_[10];
        bool writing_;
        int stripCount_;

    static void showStrip(const HwFrameComponent* component){
            uint16_t strip_len = component->strip->numPixels();
            for (uint64_t i=0; i<strip_len; i++) {
                component->strip->setPixelColor(i, component->pixels[i].red,
                    component->pixels[i].green, 
                    component->pixels[i].blue);
            }
            component->strip->show();
        }

};


#endif