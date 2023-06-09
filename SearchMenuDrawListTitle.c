#include <Agb.h>

extern u16 Bg0Bak[32*20];

void SearchMenuDrawListTitle(u16 Pos, u8 Len, u16 CharNo)
{
    u16 *bg;

    bg = Bg0Bak + Pos;
    if (CharNo == 0x161) {
        bg[-31] = 0x115;
    } else if (CharNo == 0x167) {
        bg[-30] = 0x13d;
    }

    while (Len--) *bg++ = CharNo++;
}
