#include <Agb.h>

extern u16 RfuCmd_RadioCfg(u16 param_1, u8 param_2, u8 param_3);

u16 RfuRadioCfg(u16 param_1, u8 param_2, u8 param_3)
{
    return RfuCmd_RadioCfg(param_1 | 0x20, param_2, param_3);
}
