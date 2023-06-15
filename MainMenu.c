#include <Agb.h>

extern struct Keys {
    u16 trg;
    u16 cont;
} Keys;

extern u8 Lang;
extern void *Proc;

extern void MenuMsgBlink(u8 Msg, u8 Rate);
extern void SoundPlaySfx(u8 Num);
extern void MenuMsgInit(void);
extern void SearchMenuInit(void);
extern void SearchMenu(void);

void MainMenu(void)
{
    MenuMsgBlink(6, 0x40);

    if (Keys.trg & (D_KEY | U_KEY)) {
        SoundPlaySfx(0);
        Lang ^= 1;
        *(vu16 *)REG_BG2VOFS = ~(56 + Lang * 24 - 1);
    }

    if (Keys.trg & A_BUTTON) {
        MenuMsgInit();
        SoundPlaySfx(2);
        SearchMenuInit();
        Proc = SearchMenu;
    }
}
