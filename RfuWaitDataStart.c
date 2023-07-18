#include <Agb.h>

#include "STWI_status.h"
#include "rfuStatic.h"
extern struct STWI_status STWI_status;
extern u16 STWI_send_MS_ChangeREQ(void);
extern struct rfuStatic rfuStatic;

u16 RfuWaitDataStart(u8 param_1)
{
    u16 ret = 0;

    if (param_1 == 0) {
        if (STWI_status.modeMaster == TRUE) {
            ret = STWI_send_MS_ChangeREQ();
            if (ret == 0) rfuStatic.unk_02 = 0;
        }
    } else if (STWI_status.modeMaster == FALSE) {
        rfuStatic.unk_02 = param_1;
    }

    return ret;
}
