#include <FastLED.h>

#define LED_PIN     15
#define NUM_LEDS    148
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void glowBetweenColors() {
	static uint8_t hue = 200;          // Starting hue (Purple)
	static int8_t hueDirection = 1;    // Direction of hue change

	// Define the hue range for the colors (Purple to Pink)
	uint8_t hueStart = 128;  // Aqua
	uint8_t hueEnd = 254;    // Pink

	// Update the hue value
	hue += hueDirection;

	// Reverse the direction when we reach the end hues
	if (hue <= hueStart || hue >= hueEnd) {
	hueDirection *= -1;
}

	// Fill the LED strip with the current hue, maximum saturation, and maximum brightness
	fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));

	// Update the LEDs
	FastLED.show();

	delay(100);  // Adjust this value to control the speed of the transition
}


void lightningEffect() {
	// Random chance to start a lightning event
	if (random16(0, 10000) < 50) {
		int flashes = random8(3, 6);  // Number of flashes per lightning event

		for (int i = 0; i < flashes; i++) {
		// Flash on
		fill_solid(leds, NUM_LEDS, CRGB::White);
		FastLED.show();
		delay(random8(20, 100));  // Flash duration

		// Flash off
		fill_solid(leds, NUM_LEDS, CRGB::Black);
		FastLED.show();
		delay(random8(50, 150));  // Time between flashes
		}

		// Pause before next lightning event
		delay(random16(1000, 5000));
	}
}


// Define segment parameters
#define NUM_SEGMENTS 4
#define SEGMENT_LENGTH NUM_LEDS / NUM_SEGMENTS

struct Segment {
  uint16_t startIndex;
  uint16_t endIndex;
  uint8_t blendAmount;
  int8_t blendDirection;
};

Segment segments[NUM_SEGMENTS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize segments
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    segments[i].startIndex = i * SEGMENT_LENGTH;
    segments[i].endIndex = min((i + 1) * SEGMENT_LENGTH - 1, NUM_LEDS - 1);
    segments[i].blendAmount = random8(0, 255);
    segments[i].blendDirection = random8(0, 2) == 0 ? 1 : -1;
  }
}



void glowSegments() {
  for (int i = 0; i < NUM_SEGMENTS; i++) {
    // Update blendAmount
    segments[i].blendAmount += segments[i].blendDirection;

    // Reverse direction at endpoints
    if (segments[i].blendAmount == 0 || segments[i].blendAmount == 255) {
      segments[i].blendDirection *= -1;
    }

    // Map blendAmount to hue avoiding blue hues
    uint8_t hue;
    if (segments[i].blendAmount < 128) {
      hue = map(segments[i].blendAmount, 0, 127, 128, 160);  // From aqua (128) down to red (0)
    } else {
      hue = map(segments[i].blendAmount, 128, 255, 171, 224);  // From red (0) up to pink (224)
    }

    // Fill segment with the current hue
    for (int j = segments[i].startIndex; j <= segments[i].endIndex; j++) {
      leds[j] = CHSV(hue, 255, 255);  // Keep saturation and brightness at 255 for vivid colors
    }
  }
}

void loop() {
  glowSegments();
  FastLED.show();
  delay(10);  // Adjust this value to control the speed of the transition
}