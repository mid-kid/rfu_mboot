#include <Agb.h>

extern u16 rfu_REQ_changeMasterSlave_check(u8 param_1);

u16 SearchProc_WaitData(void)
{
    return rfu_REQ_changeMasterSlave_check(0);
}
