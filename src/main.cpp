#include <IRremote.hpp>
#include "lighting.h"

Lighting lighting;
enum PALETTE state = FAIRY;

static const int UP = 0x18;
static const int DOWN = 0x52;
static const int LEFT = 0x8;
static const int RIGHT = 0x5A;
static const int ONE = 0x45;
static const int TWO = 0x46;
static const int THREE = 0x47;
static const int FOUR = 0x44;
static const int OKAY = 0x1C;



void setup() {
    Serial.begin(115200);
    IrReceiver.begin(4);
}

void loop()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
        {
            ;
        }
        else
        {
            switch (IrReceiver.decodedIRData.command)
            {
            case UP:
                lighting.clear_strip();
                state = static_cast<PALETTE>((state + 1) % lighting.NUM_PALETTES);
                break;
            case DOWN:
                lighting.clear_strip();
                state = static_cast<PALETTE>((state - 1 + lighting.NUM_PALETTES) % lighting.NUM_PALETTES);
                break;
            case LEFT:
                lighting.set_brightness(-20);
                break;
            case RIGHT:
                lighting.set_brightness(20);
                break;
            case ONE:
                lighting.select_strip(0);
                break;
            case TWO:
                lighting.select_strip(1);
                break;
            case THREE:
                lighting.select_strip(2);
                break;
            case FOUR:
                lighting.select_strip(3);
                break;
            default:
                break;
            }
        }
        IrReceiver.resume();
    }

        switch (state)
        {
        case FAIRY:
            lighting.fairy_room(lighting.fairy_palette);
            break;
        case SPIDER:
            lighting.spider_room_lightning();
            break;
        case ZOO:
            lighting.petting_zoo_room();
            break;
        case SCREAM:
            lighting.fairy_room(lighting.scream_room_palette);
            break;
        default:
            break;
        }
}

