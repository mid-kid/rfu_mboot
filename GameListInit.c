#include <Agb.h>

extern u8 GameList[0x80];
extern u8 MenuBusy;

extern void BgScClear(u16 Pos, u8 Height, u8 Width);

void GameListInit(void)
{
    MenuBusy = 0;
    BgScClear(0xe3, 8, 0x19);

    CpuClear(0, GameList, sizeof(GameList), 16);
}
