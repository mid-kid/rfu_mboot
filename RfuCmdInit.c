#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;

u32 RfuCmdInit(void)
{
    *(vu16 *)REG_RCNT = 0x100;
    *(vu16 *)REG_SIOCNT = 0x5003;
    Rfu.modeMaster = TRUE;
    Rfu.field0_0x0 = 0;
    Rfu.cmdHeader = 0;
    Rfu.cmdSize = 0;
    Rfu.field3_0x9 = 0;
    Rfu.field4_0xa = 0;
    Rfu.modeMaster = TRUE;
    Rfu.unk_07 = 0;
    Rfu.unk_08 = 0;
    Rfu.unk_10 = 0xff;
    Rfu.unk_11 = 0;
    return 0;
}
