#include <Agb.h>
#include "myFunc.h"

#include "GameInfo.h"
extern void SearchMenuDrawGame(u16 Pos, struct GameInfo *Game);
extern struct GameInfo GameList[4];
extern u8 GameListBitsNew;
extern u8 FrameCount;

void SearchMenuDrawList(u8 Blink)
{
    u8 x;
    u16 pos;

    for (pos = 0xe3, x = 0; x < 4; pos += 0x40, x++) {
        if (!(GameListBitsNew & (1 << x))) continue;

        if (!Blink || (FrameCount & 0x20) == 0) {
            SearchMenuDrawGame(pos, GameList + x);
        } else {
            mf_clearRect(pos, 1, 0x19);
        }
    }

    FrameCount++;
}
