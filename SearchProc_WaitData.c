#include <Agb.h>

extern u16 RfuWaitDataStart(u8 param_1);

u16 SearchProc_WaitData(void)
{
    return RfuWaitDataStart(0);
}
