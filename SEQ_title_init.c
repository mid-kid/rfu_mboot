#include <Agb.h>

extern u16 Bg0Bak[32*20];
extern u8 Lang;
extern u8 MainMenuFadeOut;
extern u8 MenuBusy;
extern u8 MenuState;

extern u16 *mf_drawString(u16 Pos, u16 PlttNo, const char *Srcp);
extern void mf_clearRect(u16 Pos, u8 Height, u8 Width);
extern void FrameCountReset(void);
extern void MenuMsgInit(void);
extern void mf_drawBg2_main(void);
extern void WinFade(u8 Dir);

void SEQ_title_init(void)
{
    u16 i;
    u16 charNo;
    u16 *bg;

    if (MainMenuFadeOut) {
        mf_clearRect(0x80, 2, 0x20);
        WinFade(0);
    }
    *(vu16 *)REG_DISPCNT &= ~DISP_BG1_ON;

    CpuClear(0, Bg0Bak, sizeof(Bg0Bak), 16);
    MenuMsgInit();
    mf_drawString(0xcb, 0, "ENGLISH");

    bg = Bg0Bak + (9 * 32 + 13);
    charNo = 0x10f;
    for (i = 0; i < 3; i++) {
        *bg++ = charNo++;
    }

    mf_drawBg2_main();
    *(vu16 *)REG_BG2VOFS = ~(56 + Lang * 24 - 1);
    *(vu16 *)REG_BG2HOFS = ~(65 - 1);

    WinFade(1);
    *(vu16 *)REG_DISPCNT |= DISP_BG2_ON;

    MenuState = 0xc0;
    FrameCountReset();
    MainMenuFadeOut = TRUE;
    MenuBusy = FALSE;
}
