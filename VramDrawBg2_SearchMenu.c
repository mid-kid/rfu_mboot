#include <Agb.h>

extern void VramClearBg2(void);

void VramDrawBg2_SearchMenu(u8 Size)
{
    u8 i;
    vu16 *bg;

    VramClearBg2();
    bg = (vu16 *)(BG_VRAM + 0x1000);
    for (i = 0; i < Size; i++) {
        bg[32*1] = 0x113;
        bg[32*0] = 0x113;
        bg++;
    }
}
