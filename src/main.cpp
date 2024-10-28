#include <IRremote.hpp>
#include "lighting.h"

Lighting lighting;

static const int UP = 0x18;
static const int DOWN = 0x52;
static const int LEFT = 0x8;
static const int RIGHT = 0x5A;
static const int ONE = 0x45;
static const int TWO = 0x46;
static const int THREE = 0x47;
static const int FOUR = 0x44;
static const int OKAY = 0x1C;

// Variable to store the active configuration
int activeConfig = 0;

void setup() {
    Serial.begin(115200);
    IrReceiver.begin(4);
}

void loop() {
    if (IrReceiver.decode()) {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
            switch (IrReceiver.decodedIRData.command) {
            case DOWN:
            case LEFT:
                lighting.set_brightness(-20);
                break;
            case UP:
            case RIGHT:
                lighting.set_brightness(20);
                break;
            case ONE:
                lighting.clear_strip(lighting.leds1);  // Clear leds1
                lighting.clear_strip(lighting.leds2);  // Clear leds2
                activeConfig = 1; // Set to Configuration 1: Fairy room on leds1, scream room on leds2
                break;
            case TWO:
                lighting.clear_strip(lighting.leds1);  // Clear leds1
                lighting.clear_strip(lighting.leds2);  // Clear leds2
                activeConfig = 2; // Set to Configuration 2: Petting zoo room on leds1, fairy room on leds2
                break;
            case THREE:
                lighting.clear_strip(lighting.leds1);  // Clear leds1
                lighting.clear_strip(lighting.leds2);  // Clear leds2
                activeConfig = 3; // Set to Configuration 3: Spider room on leds1, spider room lightning on leds2
                break;
            case FOUR:
                lighting.clear_strip(lighting.leds1);  // Clear leds1
                lighting.clear_strip(lighting.leds2);  // Clear leds2
                activeConfig = 4; // Set to Configuration 4: Scream room on leds1, petting zoo room on leds2
                break;
            case OKAY:
                lighting.clear_strip(lighting.leds1);  // Clear leds1
                lighting.clear_strip(lighting.leds2);  // Clear leds2
                activeConfig = 0; // No active configuration after clearing
                break;
            default:
                break;
            }
        }
        IrReceiver.resume();
    }

    // Apply the currently active configuration
    switch (activeConfig) {
    case 1:
        lighting.fairy_room(lighting.leds1);
        lighting.scream_room(lighting.leds2);
        break;
    case 2:
        lighting.scream_room(lighting.leds1);
        lighting.spider_room_lightning(lighting.leds2, 216);
        break;
    case 3:
        lighting.spider_room_lightning(lighting.leds1, 216);
        lighting.spider_room_lightning(lighting.leds2, 216);
        break;
    default:
        lighting.clear_strip(lighting.leds1);
        lighting.clear_strip(lighting.leds2);
        break;
    }
}
