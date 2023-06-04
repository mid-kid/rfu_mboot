#include <Agb.h>

void VramClearBg2(void)
{
    u8 i;
    vu16 *bg;

    bg = (vu16 *)(BG_VRAM + 0x1000);
    for (i = 0; i < 0x40; i++) *bg++ = 0; 
}
