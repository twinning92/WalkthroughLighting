#ifndef LIGHTING_H
#define LIGHTING_H

#include <FastLED.h>

enum PALETTE {
    FAIRY = 0,
    SPIDER,
    ZOO,
    SCREAM,
} palette_state;

class Lighting {
    public:
        static const int NUM_LEDS = 144 + (0.5*144);
        static const int NUM_PALETTES = 4;

        static const int MAX_STRIPS = 4;

        struct CRGB leds[MAX_STRIPS][NUM_LEDS];

        int selected_strip = 0;

        CRGBPalette16 fairy_palette;
        CRGBPalette16 petting_zoo_palette;
        CRGBPalette16 scream_room_palette;
        CRGBPalette16 spider_room_palette;

        uint8_t base_index[MAX_STRIPS] = {0};
        bool occupied[MAX_STRIPS][NUM_LEDS] = {false};

        Lighting(){
            initialize_pallettes();
            for (int i = 0; i <  MAX_STRIPS; i++){
                FastLED.addLeds<WS2812B, 15, GRB>(leds[0], NUM_LEDS);
                FastLED.addLeds<WS2812B, 27, GRB>(leds[1], NUM_LEDS);
                //FastLED.addLeds<WS2812B, 32, GRB>(leds[2], NUM_LEDS);
                //FastLED.addLeds<WS2812B, 33, GRB>(leds[3], NUM_LEDS);
                FastLED.setBrightness(50);
            }
        }

        void select_strip(int strip_index) {
            if(strip_index >= 0 && strip_index < MAX_STRIPS){
                selected_strip = strip_index;
            }
        }

        void set_brightness(int amount) {
            int current_brightness = FastLED.getBrightness();
            int new_brightness = current_brightness + amount;

            // Clamp the brightness between 0 and 255
            if (new_brightness < 0) {
                new_brightness = 0;
            } else if (new_brightness > 255) {
                new_brightness = 255;
            }

            FastLED.setBrightness(new_brightness);
            FastLED.show();
        }

        void initialize_pallettes() {
            fairy_palette = CRGBPalette16(
                CHSV(128, 255, 255),  // Aqua (hue 128)
                CHSV(128, 255, 255),  // Aqua
                CHSV(224, 255, 255),  // Pink (hue 224)
                CHSV(224, 255, 255)   // Pink
            );
            petting_zoo_palette = CRGBPalette16(
                CRGB::ForestGreen,
                CRGB::Purple,
                CRGB::DarkRed
            );

            scream_room_palette = CRGBPalette16(
                CRGB::DarkRed,
                CRGB::Red,
                CRGB::OrangeRed
            );
        }
        void createSparkle(uint8_t start_index, uint8_t sparkle_size, CRGB sparkle_colour) {
        uint8_t center = start_index + (sparkle_size / 2);
        uint8_t max_distance = (sparkle_size) / 2;

        for (uint8_t offset = 0; offset < sparkle_size; offset++) {
            uint8_t ledIndex = start_index + offset;
            uint8_t distance = abs(int(ledIndex) - int(center));
            uint8_t brightness = 255 - ((distance * 255) / max_distance);
            leds[selected_strip][ledIndex] = sparkle_colour;
            leds[selected_strip][ledIndex].nscale8(brightness);
            occupied[selected_strip][ledIndex] = true;
        }
    }

    void clear_strip() {
        FastLED.showColor(CRGB::Black);
    }

    bool isRangeOccupied(uint8_t start_index, uint8_t sparkle_size) {
        for (uint8_t offset = 0; offset < sparkle_size; offset++) {
            uint8_t ledIndex = start_index + offset;
            if (occupied[selected_strip][ledIndex]) {
                return true;
            }
        }
        return false;
    }

    void petting_zoo_room() {
        EVERY_N_MILLIS(30) {
            fadeToBlackBy(leds[selected_strip], NUM_LEDS, 1);
        }

        EVERY_N_SECONDS(4) {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
                occupied[selected_strip][i] = false;
            }

            int num_sparkles = random8(8, 20);

            for (uint8_t i = 0; i < num_sparkles; i++) {
                uint8_t sparkle_size = random8(10, 25);
                uint8_t start_index;

                uint8_t attempts = 0;
                do {
                    start_index = random8(NUM_LEDS - sparkle_size);
                    attempts++;
                } while (isRangeOccupied(start_index, sparkle_size) && attempts < 10);

                if (attempts < 10) {
                    CRGB sparkle_colour = ColorFromPalette(petting_zoo_palette, random8(), 255, NOBLEND);
                    createSparkle(start_index, sparkle_size, sparkle_colour);
                }
            }
        }

        FastLED.show();
    }

    void spider_room() {
        int index = 0;

        for (int r = 0; r <= 255; r += 10) {
            for (int g = 0; g <= 255; g += 10) {
                CRGB color = CRGB(r, g, 255);
                leds[selected_strip][index] = color;

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

    void spider_room_lightning() {
            int flashes = random(3, 8); // Random number of flashes per strike

            for (int i = 0; i < flashes; i++) {
                // Set a random brightness for the flash
                // int flashBrightness = random(200, 255);
                // FastLED.setBrightness(flashBrightness);

                // Set a random duration for the flash
                int flashDuration = random(20, 250); // Flash duration in milliseconds

                // Flash the LEDs with white light
                int k = random(0,10);
                CRGB flash_colour;
                if (k < 8)
                {
                    flash_colour = CRGB::White;
                }
                else
                {
                    flash_colour = CRGB(80,0,255);
                }
                fill_solid(leds[selected_strip], NUM_LEDS, flash_colour);
                FastLED.show();
                delay(flashDuration);

                // Turn off the LEDs
                fill_solid(leds[selected_strip], NUM_LEDS, CRGB::Black);
                FastLED.show();

                // Random pause between flashes
                int pause = random(50, 220);
                delay(pause);
            }

            // Optional: Fade out effect after the spider_room_lightning flashes
            int fadeOutTime = random(500, 1000); // Fade-out duration in milliseconds
            int BRIGHTNESS = FastLED.getBrightness();
            for (int i = BRIGHTNESS; i >= 0; i--) {
                FastLED.setBrightness(i);
                FastLED.show();
                delay(fadeOutTime / BRIGHTNESS);
            }

            // Reset brightness to maximum for the next spider_room_lightning strike
            FastLED.setBrightness(BRIGHTNESS);
        }

    void fairy_room(CRGBPalette16 palette) {
        EVERY_N_MILLIS(50) {
            base_index[selected_strip]++;

            for (int i = 0; i < NUM_LEDS; i++) {
                uint8_t colorIndex = (base_index[selected_strip] + i * (255 / NUM_LEDS)) % 255;
                leds[selected_strip][i] = ColorFromPalette(palette, colorIndex, 255, LINEARBLEND);
            }

            FastLED.show();
        }
    }
};

#endif