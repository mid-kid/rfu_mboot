#include <Agb.h>
#include "myFunc.h"

extern u8 GameList[0x80];
extern u8 MenuBusy;

void menu_initGameList(void)
{
    MenuBusy = 0;
    mf_clearRect(0xe3, 8, 0x19);

    CpuClear(0, GameList, sizeof(GameList), 16);
}
