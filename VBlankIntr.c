#include <Agb.h>

extern void SoundMain(void);
extern void FUN_03002d4c(void);
extern u16 BgBak[32*20];

void VBlankIntr(void)
{
    DmaArrayCopy(3, BgBak, VRAM, 32);

    SoundMain();
    FUN_03002d4c();

    *(vu16 *)INTR_CHECK_BUF = 1;
}
