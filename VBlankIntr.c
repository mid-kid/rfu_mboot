#include <Agb.h>

extern void SoundMain(void);
extern void RfuVBlank(void);
extern u16 BgBak[32*20];

void VBlankIntr(void)
{
    DmaArrayCopy(3, BgBak, VRAM, 32);

    SoundMain();
    RfuVBlank();

    *(vu16 *)INTR_CHECK_BUF = 1;
}
