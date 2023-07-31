#include <Agb.h>

extern void Sio32Intr(void);
extern void VBlankIntr(void);
extern void SoundMain(void);
extern void rfu_syncVBlank(void);
extern u16 Bg0Bak[32*20];

void (*const IntrTable[])(void) = {
    Sio32Intr,
    VBlankIntr
};

void VBlankIntr(void)
{
    DmaArrayCopy(3, Bg0Bak, VRAM, 32);

    SoundMain();
    rfu_syncVBlank();

    *(vu16 *)INTR_CHECK_BUF = 1;
}
