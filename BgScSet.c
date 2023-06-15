#include <Agb.h>

extern u16 BgBak[32*20];

u16 *BgScSet(u16 Pos, u16 PlttNo, const char *Srcp)
{
    u16 *bg;

    bg = BgBak + Pos;
    while (*Srcp != 0) {
        *bg++ = *Srcp++ | PlttNo << 0xc;
    }
    return bg;
}
