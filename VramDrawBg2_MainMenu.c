#include <Agb.h>

extern void VramClearBg2(void);

void VramDrawBg2_MainMenu(void)
{
    u8 i;
    u16 *bg;
    u16 tile;

    VramClearBg2();
    bg = (vu16 *)(BG_VRAM + 0x1000);

    tile = 0x2143;
    for (i = 0; i < 4; i++) *bg++ = tile++; 
    for (i = 0; i < 5; i++) *bg++ = tile; 
    tile = (tile - 1) | 0x400;
    for (i = 0; i < 4; i++) *bg++ = tile--; 
}
