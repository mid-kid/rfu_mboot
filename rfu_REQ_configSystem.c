#include <Agb.h>

extern u16 STWI_send_SystemConfigREQ(u16 param_1, u8 param_2, u8 param_3);

u16 rfu_REQ_configSystem(u16 param_1, u8 param_2, u8 param_3)
{
    return STWI_send_SystemConfigREQ(param_1 | 0x20, param_2, param_3);
}
