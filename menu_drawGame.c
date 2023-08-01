#include <Agb.h>
#include "myFunc.h"

#include "GameInfo.h"

// Function definition not included in myFunc.h, defaults to int
extern void mf_clearGame(int Pos);

void menu_drawGame(u16 Pos, struct GameInfo *Game)
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
