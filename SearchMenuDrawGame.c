#include <Agb.h>

#include "GameInfo.h"

extern void BgScClearGame();
extern u16 *BgScSet(u16 Pos, u16 PlttNo, char *Srcp);

void SearchMenuDrawGame(u16 Pos, struct GameInfo *Game)
{
    if (!Game->gameName[0]) {
        BgScClearGame(Pos);
    } else {
        BgScSet(Pos, 0, Game->gameName);
    }

    if (!Game->userName[0]) {
        BgScClearGame(Pos + 15);
    } else {
        BgScSet(Pos + 15, 0, Game->userName);
    }
}
