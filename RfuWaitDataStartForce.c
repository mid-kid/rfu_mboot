#include <Agb.h>

#include "MbootTmp.h"
extern struct MbootTmp MbootTmp;
extern u16 RfuCmd_WaitData(void);

u16 RfuWaitDataStartForce(void)
{
    u16 ret = 0;

    if (MbootTmp.unk_02 == 0) {
        ret = RfuCmd_WaitData();
    } else {
        MbootTmp.unk_02 = 0;
    }

    return ret;
}
