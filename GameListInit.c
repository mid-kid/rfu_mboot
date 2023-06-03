#include <Agb.h>

extern u8 GameList[0x80];
extern u8 u8_03005754;

extern void BgScFill(u16 Pos, u8 Height, u8 Width);

void GameListInit(void)
{
    u8_03005754 = 0;
    BgScFill(0xe3, 8, 0x19);

    CpuClear(0, GameList, sizeof(GameList), 16);
}
