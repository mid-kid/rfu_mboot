#include <Agb.h>

#include "GameInfo.h"
extern void WinFade(u8 Dir);
extern void VramDrawBg2_SearchMenu(u8 Len);
extern void SearchMenuDrawListTitle(u16 Pos, u8 Len, u16 CharNo);
extern void GameNameInit(void);
extern u32 MbootReset(void);
extern u8 LZ_450c[];
extern u16 Bg0Bak[32*20];
extern u8 Lang;
extern struct GameInfo GameList[4];
extern u8 SearchMenuEnd;
extern u8 SearchMenuCursor;
extern u16 SearchMenuTimer;
extern u8 SearchMenuErrorTimer;
extern u8 SearchMenuErrorMsg;
extern u8 MenuState;
#define SEARCH_BOOT 0x0a

void SearchMenuInit(void)
{
    u8 x;

    WinFade(0);

    VramDrawBg2_SearchMenu(0x18);
    *(vu16 *)REG_BG2VOFS = -52;
    *(vu16 *)REG_BG2HOFS = -24;
    DmaClear(3, 0, Bg0Bak, sizeof(Bg0Bak), 16);
    LZ77UnCompVram(&LZ_450c, (void *)0x6000800);
    VBlankIntrWait();

    *(vu16 *)REG_DISPCNT |= DISP_BG1_ON;
    WinFade(1);
    *(vu16 *)REG_DISPCNT |= DISP_BG2_ON;

    if (Lang == 0) {
        SearchMenuDrawListTitle(0xa6, 8, 0x151);
        SearchMenuDrawListTitle(0xb2, 8, 0x159);
    } else {
        SearchMenuDrawListTitle(0xa7, 6, 0x161);
        SearchMenuDrawListTitle(0xb3, 7, 0x167);
    }

    for (x = 0; x < 4; x++) GameList[x].beaconID = 0;
    MbootReset();
    SearchMenuEnd = FALSE;
    SearchMenuCursor = 0;
    SearchMenuErrorTimer = 0;
    SearchMenuErrorMsg = -1;
    MenuState = SEARCH_BOOT;
    GameNameInit();
}
