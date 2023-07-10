#include <Agb.h>

extern u16 Bg0Bak[32*20];

void BgScClearGame(u16 Pos)
{
    u8 x;
    u16 *bg;

    bg = Bg0Bak + Pos;
    for (x = 0; x < 8; x++) {
        *bg++ = 0x142;
    }
}
