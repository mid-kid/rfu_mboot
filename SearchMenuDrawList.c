#include <Agb.h>

#include "GameInfo.h"
extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern void SearchMenuDrawGame(u16 Pos, struct GameInfo *Game);
extern struct GameInfo GameList[4];
extern u8 GameListBits;
extern u8 FrameCount;

void SearchMenuDrawList(u8 Blink)
{
    u8 x;
    u16 pos;

    for (pos = 0xe3, x = 0; x < 4; pos += 0x40, x++) {
        if (!(GameListBits & (1 << x))) continue;

        if (!Blink || (FrameCount & 0x20) == 0) {
            SearchMenuDrawGame(pos, GameList + x);
        } else {
            BgScClear(pos, 1, 0x19);
        }
    }

    FrameCount++;
}
