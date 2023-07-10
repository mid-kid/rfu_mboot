#include <Agb.h>

extern u16 Bg0Bak[32*20];

void BgScClear(u16 Pos, u8 Height, u8 Width)
{
    u8 y, x;
    u16 *bg;

    for (y = 0; y < Height; y++) {
        bg = Bg0Bak + Pos + y * 32;
        for (x = 0; x < Width; x++) {
            *bg++ = 0x20;
        }
    }
}
