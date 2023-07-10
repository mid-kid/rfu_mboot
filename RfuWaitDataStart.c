#include <Agb.h>

#include "Rfu.h"
#include "MbootTmp.h"
extern struct Rfu Rfu;
extern u16 RfuCmd_WaitData(void);
extern struct MbootTmp MbootTmp;

u16 RfuWaitDataStart(u8 param_1)
{
    u16 ret = 0;

    if (param_1 == 0) {
        if (Rfu.modeMaster == TRUE) {
            ret = RfuCmd_WaitData();
            if (ret == 0) MbootTmp.unk_02 = 0;
        }
    } else if (Rfu.modeMaster == FALSE) {
        MbootTmp.unk_02 = param_1;
    }

    return ret;
}
