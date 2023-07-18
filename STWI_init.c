#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;

u32 STWI_init(void)
{
    *(vu16 *)REG_RCNT = 0x100;
    *(vu16 *)REG_SIOCNT = 0x5003;
    STWI_status.modeMaster = TRUE;
    STWI_status.state = 0;
    STWI_status.cmdHeader = 0;
    STWI_status.cmdSize = 0;
    STWI_status.field3_0x9 = 0;
    STWI_status.error = 0;
    STWI_status.modeMaster = TRUE;
    STWI_status.unk_07 = 0;
    STWI_status.unk_08 = 0;
    STWI_status.timer = -1;
    STWI_status.unk_11 = 0;
    return 0;
}
