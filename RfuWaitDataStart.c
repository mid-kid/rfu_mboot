#include <Agb.h>

#include "STWI_status.h"
#include "MbootTmp.h"
extern struct STWI_status STWI_status;
extern u16 RfuCmd_WaitData(void);
extern struct MbootTmp MbootTmp;

u16 RfuWaitDataStart(u8 param_1)
{
    u16 ret = 0;

    if (param_1 == 0) {
        if (STWI_status.modeMaster == TRUE) {
            ret = RfuCmd_WaitData();
            if (ret == 0) MbootTmp.unk_02 = 0;
        }
    } else if (STWI_status.modeMaster == FALSE) {
        MbootTmp.unk_02 = param_1;
    }

    return ret;
}
