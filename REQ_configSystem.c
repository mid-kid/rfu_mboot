#include <Agb.h>

extern u16 rfu_REQ_configSystem(u16 param_1, u8 param_2, u8 param_3);

u16 REQ_configSystem(void)
{
    return rfu_REQ_configSystem(0x1c, 4, 0x20);
}
