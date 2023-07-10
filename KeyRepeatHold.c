#include <Agb.h>

extern struct key {
    u16 trg;
    u16 cont;
} key;

extern u8 KeyTimers[4];

void KeyRepeatHold(void)
{
    u16 bit;
    u8 i;

    for (i = 0; i < 4; i++) {
        bit = 1 << (i + 4);
        if (key.cont & bit) {
            KeyTimers[i]++;
            if (KeyTimers[i] == 20) {
                key.trg |= bit;
                KeyTimers[i] = 15;
            }
        } else {
            KeyTimers[i] = 0;
        }
    }
}
