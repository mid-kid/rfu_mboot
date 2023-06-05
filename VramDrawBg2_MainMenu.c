#include <Agb.h>

extern void VramClearBg2(void);

void VramDrawBg2_MainMenu(void)
{
    u8 i;
    u16 charNo;
    vu16 *bg;

    VramClearBg2();
    bg = (vu16 *)(BG_VRAM + 0x1000);

    charNo = 0x2143;
    for (i = 0; i < 4; i++) *bg++ = charNo++; 
    for (i = 0; i < 5; i++) *bg++ = charNo; 
    charNo = (charNo - 1) | 0x400;
    for (i = 0; i < 4; i++) *bg++ = charNo--; 
}
