#ifndef LIGHTING_H
#define LIGHTING_H

#include <FastLED.h>

class Lighting {
public:
    static const int NUM_LEDS = 216;
    CRGB leds1[NUM_LEDS];
    CRGB leds2[NUM_LEDS];
    CRGB leds3[NUM_LEDS];

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
        FastLED.addLeds<WS2812B, 32, GRB>(leds3, NUM_LEDS);
        FastLED.setBrightness(255);
    }

    void set_brightness(int amount) {
        int current_brightness = FastLED.getBrightness();
        int new_brightness = constrain(current_brightness + amount, 0, 255);
        FastLED.setBrightness(new_brightness);
        FastLED.show();
    }

    void clear_strip(CRGB* leds) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
    }

    void create_sparkle(CRGB* leds, uint8_t start_index, uint8_t sparkle_size, CRGB sparkle_colour) {
        uint8_t center = start_index + (sparkle_size / 2);
        uint8_t max_distance = (sparkle_size) / 2;

        for (uint8_t offset = 0; offset < sparkle_size; offset++) {
            uint8_t ledIndex = start_index + offset;
            uint8_t distance = abs(int(ledIndex) - int(center));
            uint8_t brightness = 255 - ((distance * 255) / max_distance);
            leds[ledIndex] = sparkle_colour;
            leds[ledIndex].nscale8(brightness);
        }
    }

    void petting_zoo_room(CRGB* leds) {
        fadeToBlackBy(leds, NUM_LEDS, 1);
        FastLED.show();

        int num_sparkles = random8(8, 20);
        for (uint8_t i = 0; i < num_sparkles; i++) {
            uint8_t sparkle_size = random8(10, 25);
            uint8_t start_index = random8(NUM_LEDS - sparkle_size);
            CRGB sparkle_colour = ColorFromPalette(petting_zoo_palette, random8(), 255, NOBLEND);
            create_sparkle(leds, start_index, sparkle_size, sparkle_colour);
        }

        FastLED.show();
    }

    void ghoul_room(CRGB* leds) {
        int index = 0;
        for (int r = 0; r <= 255; r += 10) {
            for (int g = 0; g <= 255; g += 10) {
                CRGB color = CRGB(r, g, 255);
                leds[index] = color;
                index++;
                if (index >= NUM_LEDS) {
                    FastLED.show();
                    delay(1000);
                    index = 0;
                }
            }
        }
        FastLED.show();
    }

void spider_room_lightning(CRGB* leds, int numLeds, int flashes = 10, int frequency = 10) {
    int ledStart, ledLen, dimmer;

    // Determine starting location of flash
    ledStart = random8(numLeds);                
    // Determine length of flash (ensuring it does not exceed the strip length)
    ledLen = random8(numLeds - ledStart);       

    // Iterate through the number of flashes for this lightning strike
    for (int flashCounter = 0; flashCounter < random8(3, flashes); flashCounter++) {
        // Set dimmer value based on whether it's the first flash (leader) or a return stroke
        if (flashCounter == 0) dimmer = 5;      
        else dimmer = random8(1, 3);            

        // Randomly choose either white or purple for each flash
        CRGB flashColor = random(0, 10) <= 7 ? CRGB::White : CRGB(80, 0, 255);

        // Fill the selected section of LEDs with the chosen flash color, scaled by dimmer
        fill_solid(leds + ledStart, ledLen, flashColor);
        for (int i = ledStart; i < ledStart + ledLen; i++) {
            leds[i].nscale8(255 / dimmer); // Adjust brightness of the flash
        }
        FastLED.show();                         
        delay(random8(5, 20));                  

        // Clear the section of LEDs to create a blinking effect
        fill_solid(leds + ledStart, ledLen, CRGB::Black);
        FastLED.show();

        // Delay between the leader and return strokes
        if (flashCounter == 0) delay(150);      
        delay(50 + random8(100));               
    }

    // Delay between lightning strikes
    delay(random8(frequency) * 100);            
}




    void fairy_room(CRGB* leds) {
        static uint8_t base_index = 0;
        base_index++;
        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t colorIndex = (base_index + i * (255 / NUM_LEDS)) % 255;
            leds[i] = ColorFromPalette(fairy_palette, colorIndex, 255, LINEARBLEND);
        }
        FastLED.show();
    }

    void scream_room(CRGB* leds) {
        static uint8_t base_index = 0;
        base_index++;
        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t colorIndex = (base_index + i * (255 / NUM_LEDS)) % 255;
            leds[i] = ColorFromPalette(scream_room_palette, colorIndex, 255, LINEARBLEND);
        }
        FastLED.show();
    }

    void glow_white(CRGB* leds) {
        fill_solid(leds, NUM_LEDS, CRGB::White);
        FastLED.show();
    }
};

#endif
