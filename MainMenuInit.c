#include <Agb.h>

extern char StringEnglish[];
extern u16 BgBak[32*20];
extern u8 Lang;
extern u8 MainMenuFadeOut;
extern u8 MenuBusy;
extern u8 MenuState;

extern u16 *BgScSet(u16 Pos, u16 PlttNo, char *Srcp);
extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern void FrameCountReset(void);
extern void MenuMsgInit(void);
extern void VramDrawBg2_MainMenu(void);
extern void WinFade(u8 Dir);

void MainMenuInit(void)
{
    u16 i;
    u16 charNo;
    u16 *bg;

    if (MainMenuFadeOut) {
        BgScClear(0x80, 2, 0x20);
        WinFade(0);
    }
    *(vu16 *)REG_DISPCNT &= ~DISP_BG1_ON;

    CpuClear(0, BgBak, sizeof(BgBak), 16);
    MenuMsgInit();
    BgScSet(0xcb, 0, StringEnglish);

    bg = BgBak + (9 * 32 + 13);
    charNo = 0x10f;
    for (i = 0; i < 3; i++) {
        *bg++ = charNo++;
    }

    VramDrawBg2_MainMenu();
    *(vu16 *)REG_BG2VOFS = ~(56 + Lang * 24 - 1);
    *(vu16 *)REG_BG2HOFS = ~(65 - 1);

    WinFade(1);
    *(vu16 *)REG_DISPCNT |= DISP_BG2_ON;

    MenuState = 0xc0;
    FrameCountReset();
    MainMenuFadeOut = TRUE;
    MenuBusy = FALSE;
}
