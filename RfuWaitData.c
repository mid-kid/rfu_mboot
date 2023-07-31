#include <Agb.h>

extern u8 rfu_REQ_changeMasterSlave_check(u8 param_1);
extern u8 rfu_getMasterSlave(void);

void RfuWaitData(void)
{
    rfu_REQ_changeMasterSlave_check(1);
    while (!rfu_getMasterSlave());
}
