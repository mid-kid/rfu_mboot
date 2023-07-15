#include <Agb.h>

#include "GameInfo.h"
extern void mf_clearRect(u16 Pos, u8 Height, u8 Width);
extern struct GameInfo GameList[4];
extern u8 SearchMenuCursor;

void SearchMenuClearGame(void)
{
    u8 x;

    for (x = 0; x < 4; x++) {
        if (x == SearchMenuCursor) continue;
        GameList[x].beaconID = 0;
        mf_clearRect(x * 0x40 + 0xe3, 1, 0x19);
    }
}
