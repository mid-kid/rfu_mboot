#include <Agb.h>

extern u32 RfuBoot(void);

u16 SearchProc_Boot(void)
{
    if (RfuBoot() == 0x8001) {
        return 0;
    } else {
        return 1;
    }
}
