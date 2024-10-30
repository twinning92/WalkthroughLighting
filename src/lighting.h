#ifndef LIGHTING_H
#define LIGHTING_H

#include <FastLED.h>

class Lighting {
public:
    static const int NUM_LEDS = 237;
    CRGB leds1[NUM_LEDS];
    CRGB leds2[NUM_LEDS];

    CRGBPalette16 fairy_palette = CRGBPalette16(
        CHSV(128, 255, 255),  // Aqua (hue 128)
        CHSV(128, 255, 255),  // Aqua
        CHSV(224, 255, 255),  // Pink (hue 224)
        CHSV(224, 255, 255)   // Pink
    );
    CRGBPalette16 petting_zoo_palette = CRGBPalette16(
        CRGB::ForestGreen,
        CRGB::Purple,
        CRGB::DarkRed
    );
    CRGBPalette16 scream_room_palette = CRGBPalette16(
        CRGB::DarkRed,
        CRGB::Red,
        CRGB::OrangeRed
    );

    Lighting() {
        FastLED.addLeds<WS2812B, 15, GRB>(leds1, NUM_LEDS);
        FastLED.addLeds<WS2812B, 27, GRB>(leds2, NUM_LEDS);
        FastLED.setBrightness(255);
    }

    void set_brightness(int amount) {
        int current_brightness = FastLED.getBrightness();
        int new_brightness = constrain(current_brightness + amount, 0, 255);
        FastLED.setBrightness(new_brightness);
    }

    void clear_strip(CRGB* leds) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
    }

void spider_room_lightning(CRGB* leds, int numLeds, int flashes = 10, int frequency = 15) {
    int ledStart, ledLen, dimmer;
    
    // Determine starting location of flash
    ledStart = random8(numLeds - (numLeds / 4));                
    // Determine length of flash (ensuring it does not exceed the strip length)
    ledLen = random8((numLeds / 4), (numLeds - ledStart));     

    // Iterate through the number of flashes for this lightning strike
    for (int flashCounter = 0; flashCounter < random8(3, flashes); flashCounter++) {
        // Set dimmer value based on whether it's the first flash (leader) or a return stroke
        if (flashCounter == 0) dimmer = 7;      
        else dimmer = random8(1, 2);            

        // Randomly choose either white or purple for each flash
        CRGB flashColor = random(0, 2) == 0 ? CRGB::White : CRGB(80, 0, 255);

        // Fill the selected section of LEDs with the chosen flash color, scaled by dimmer
        fill_solid(leds + ledStart, ledLen, flashColor);
        for (int i = ledStart; i < ledStart + ledLen; i++) {
            leds[i].nscale8(255 / dimmer); // Adjust brightness of the flash
        }
        FastLED.show();                         
        delay(random8(5, 60));                  

        // Clear the section of LEDs to create a blinking effect
        fill_solid(leds + ledStart, ledLen, CRGB::Black);
        FastLED.show();

        // Delay between the leader and return strokes
        if (flashCounter == 0) delay(random16(100,350));      
        delay(50 + random8(100));               
    }

    // Delay between lightning strikes
    delay(random8(3, frequency) * 100);            
}

    void fairy_room(CRGB* leds) {
            static uint8_t base_index = 0;
            base_index++;
            for (int i = 0; i < NUM_LEDS; i++) {
                uint8_t colorIndex = (base_index + i * (255 / NUM_LEDS)) % 255;
                leds[i] = ColorFromPalette(fairy_palette, colorIndex, 255, LINEARBLEND);
            }
    }

    void scream_room(CRGB* leds) {
            static uint8_t base_index = 0;
            base_index++;
            for (int i = 0; i < NUM_LEDS; i++) {
                uint8_t colorIndex = (base_index + i * (255 / NUM_LEDS)) % 255;
                leds[i] = ColorFromPalette(scream_room_palette, colorIndex, 255, LINEARBLEND);
            }
        
    }

    void default_room(CRGB* leds) {
        fill_solid(leds, NUM_LEDS, CRGB::Green);
    }

    void glow_white(CRGB* leds, CRGB colour) {
        fill_solid(leds, NUM_LEDS, colour);
    }
};

#endif
