#include <Agb.h>

void WinInit(void)
{
    *(vu16 *)REG_WIN0H = 0xff;
    *(vu16 *)REG_WIN0V = 0xff;
    *(vu16 *)REG_WIN0 = 0x3f;
    *(vu16 *)REG_WINOUT = 0;
    *(vu16 *)REG_BLDCNT = 0x2857;
    *(vu16 *)REG_BLDALPHA = 0x1f00;
}
