#include <Agb.h>

extern u16 RfuRadioCfg(u16 param_1, u8 param_2, u8 param_3);

u16 SearchProc_RadioCfg(void)
{
    return RfuRadioCfg(0x1c, 4, 0x20);
}
