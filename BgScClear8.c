#include <Agb.h>

extern u16 BgBak[32*20];

void BgScClear8(u16 Pos)
{
    u8 x;
    u16 *bg;

    bg = BgBak + Pos;
    for (x = 0; x < 8; x++) {
        *bg++ = 0x142;
    }
}
