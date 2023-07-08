#include <Agb.h>

extern u16 RfuMbootDLStart(u8 param_1, u8 param_2, u16 param_3, u16 *GameID, u32 param_5);

u16 MbootDLStart1(u8 param_1, u16 param_2, u16 *GameID, u32 param_4)
{
    return RfuMbootDLStart(0x20, param_1, param_2, GameID, param_4);
}
