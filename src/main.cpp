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
static const int FIVE = 0x40;
static const int SIX = 0x43;
static const int SEVEN = 0x7;
static const int EIGHT = 0x15;
static const int NINE = 0x9;
static const int ZERO = 0x19;
static const int STAR = 0xFF;
static const int HASH = 0xD;

static const int OKAY = 0x1C;

// Variable to store the active configuration
int activeConfig = 0;

bool switch_strips = false;

void setup()
{
    Serial.begin(115200);
    IrReceiver.begin(4);
}

void loop()
{
    if (IrReceiver.decode())
    {
        if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
        {
            lighting.clear_strip(lighting.leds1);
            lighting.clear_strip(lighting.leds2);
            switch (IrReceiver.decodedIRData.command)
            {
            case DOWN:
                lighting.set_brightness(-40);
                break;
            case LEFT:
                lighting.set_brightness(-20);
                break;
            case UP:
                lighting.set_brightness(40);
                break;
            case RIGHT:
                lighting.set_brightness(20);
                break;
            case ONE:
                switch_strips = !switch_strips;
                activeConfig = 1;
                break;
            case TWO:
                switch_strips = !switch_strips;
                activeConfig = 2;
                break;
            case THREE:
                activeConfig = 3;
                break;
            case FOUR:
                activeConfig = 4;
                break;
            case FIVE:
                activeConfig = 5;
                break;
            case SIX:
                activeConfig = 6;
            case OKAY:
                lighting.clear_strip(lighting.leds1);
                lighting.clear_strip(lighting.leds2);
                activeConfig = 7;
                break;
            default:
                break;
            }
        }
        IrReceiver.resume();
    }

    switch (activeConfig)
    {
    case 1:
        if (switch_strips)
        {
            EVERY_N_MILLIS(100)
            {
                lighting.fairy_room(lighting.leds1);
            }
            EVERY_N_MILLIS(100)
            {
                lighting.scream_room(lighting.leds2);
            }
        }
        else
        {
            EVERY_N_MILLIS(100)
            {
                lighting.fairy_room(lighting.leds2);
            }
            EVERY_N_MILLIS(100)
            {
                lighting.scream_room(lighting.leds1);
            }
        }
        break;
    case 2:
        if (switch_strips)
        {
            EVERY_N_MILLIS(100)
            {
                lighting.scream_room(lighting.leds1);
            }
            lighting.spider_room_lightning(lighting.leds2, 237);
        }
        else
        {
            EVERY_N_MILLIS(100)
            {
                lighting.scream_room(lighting.leds2);
            }
            lighting.spider_room_lightning(lighting.leds1, 237);
        }
        Serial.println(switch_strips);
        break;
    case 3:
        lighting.spider_room_lightning(lighting.leds1, 237);
        lighting.spider_room_lightning(lighting.leds2, 237);
        break;
    case 4:
        lighting.glow_white(lighting.leds1, CRGB::White);
        lighting.glow_white(lighting.leds2, CRGB::White);
        break;
    case 5:
        EVERY_N_MILLIS(100)
        {
            lighting.fairy_room(lighting.leds1);
        }
        EVERY_N_MILLIS(100)
        {
            lighting.fairy_room(lighting.leds2);
        }
        break;
    case 6:
    case 7:
    default:
        lighting.default_room(lighting.leds1);
        lighting.default_room(lighting.leds2);
        break;
    }
    FastLED.show();
}
