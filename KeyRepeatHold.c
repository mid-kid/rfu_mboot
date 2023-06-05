#include <Agb.h>

extern struct Keys {
    u16 trg;
    u16 cont;
} Keys;

extern u8 KeyTimers[4];

void KeyRepeatHold(void)
{
    u16 bit;
    u8 i;

    for (i = 0; i < 4; i++) {
        bit = 1 << (i + 4);
        if (Keys.cont & bit) {
            KeyTimers[i]++;
            if (KeyTimers[i] == 20) {
                Keys.trg |= bit;
                KeyTimers[i] = 15;
            }
        } else {
            KeyTimers[i] = 0;
        }
    }
}
