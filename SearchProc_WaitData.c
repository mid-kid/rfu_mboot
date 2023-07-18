#include <Agb.h>

extern u16 rfu_REQ_changeMasterSlave(u8 param_1);

u16 SearchProc_WaitData(void)
{
    return rfu_REQ_changeMasterSlave(0);
}
