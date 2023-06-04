#include <Agb.h>

void WinFade(u8 Dir)
{
    u16 i;
    u16 val;

    if (Dir == 0) *(vu16 *)REG_DISPCNT &= 0xfbff; 

    i = 0;
    while (i <= 0x10) {
        VBlankIntrWait();
        if (i == 0x10) {
            val = 0;
        } else {
            //val = 0x10 - i;
            val = (~i & 0xf) + 1;
        }
        if (Dir) {
            *(vu16 *)REG_BLDALPHA = (val << 8) | i;
        } else {
            *(vu16 *)REG_BLDALPHA = val | (i << 8);
        }
        i++;
        VBlankIntrWait();
    }
}
