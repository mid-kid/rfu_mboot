#include <Agb.h>
#include "myFunc.h"

#include "GameInfo.h"
extern struct GameInfo GameList[4];
extern u8 SearchMenuCursor;

void menu_clearGameList(void)
{
    u8 x;

    for (x = 0; x < 4; x++) {
        if (x == SearchMenuCursor) continue;
        GameList[x].beaconID = 0;
        mf_clearRect(x * 0x40 + 0xe3, 1, 0x19);
    }
}
