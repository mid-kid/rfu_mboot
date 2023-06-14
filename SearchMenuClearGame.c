#if 0
__asm__("
.section .text
@.global SearchMenuClearGame
.type SearchMenuClearGame, function
.thumb_func
SearchMenuClearGame:
.2byte 0xb530,0x2400,0x4d0c,0x480d,0x7800,0x4284,0xd00c,0x0160,0x1940,0x2100,0x8001,0x05a0,0x21e3,0x0409,0x1840,0x0c00,0x2101,0x2219,0xf000,0xfa1e,0x1c60,0x0600,0x0e04,0x2c03,0xd9e9,0xbc30,0xbc01,0x4700,0x56a0,0x0300,0x5691,0x0300
.size SearchMenuClearGame, .-SearchMenuClearGame
");
#else

#include <Agb.h>

#include "GameInfo.h"
extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern struct GameInfo GameList[4];
extern u8 SearchMenuCursor;

void SearchMenuClearGame(void)
{
    u8 x;

    for (x = 0; x < 4; x++) {
        if (x == SearchMenuCursor) continue;
        GameList[x].beaconID = 0;
        BgScClear(x * 0x40 + 0xe3, 1, 0x19);
    }
}

#endif
