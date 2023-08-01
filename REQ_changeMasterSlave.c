#include <Agb.h>

extern u16 rfu_REQ_changeMasterSlave_check(u8 param_1);

u16 REQ_changeMasterSlave(void)
{
    return rfu_REQ_changeMasterSlave_check(0);
}
