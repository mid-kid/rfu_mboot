#include <Agb.h>

#include "GameInfo.h"

extern void mf_clearGame();
extern u16 *mf_drawString(u16 Pos, u16 PlttNo, char *Srcp);

void SearchMenuDrawGame(u16 Pos, struct GameInfo *Game)
{
    if (!Game->gameName[0]) {
        mf_clearGame(Pos);
    } else {
        mf_drawString(Pos, 0, Game->gameName);
    }

    if (!Game->userName[0]) {
        mf_clearGame(Pos + 15);
    } else {
        mf_drawString(Pos + 15, 0, Game->userName);
    }
}
