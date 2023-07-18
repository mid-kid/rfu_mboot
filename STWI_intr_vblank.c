#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;

u32 STWI_intr_vblank(void)
{
    if (STWI_status.modeMaster == TRUE) return 0;

    if (STWI_status.timer != (u8)-1) STWI_status.timer++;

    if (STWI_status.timer == 6) {
        STWI_status.state = 5;
        STWI_status.cmdHeader = 0;
        STWI_status.cmdSize = 0;
        STWI_status.field3_0x9 = 0;
        STWI_status.error = 0;
        STWI_status.unk_07 = 0;
        STWI_status.unk_08 = 0;
        *(vu32 *)REG_SIODATA32 = 0x80000000;
        *(vu16 *)REG_SIOCNT = 0;
        *(vu16 *)REG_SIOCNT = 0x5003;
        *(vu16 *)REG_SIOCNT = 0x5082;
        STWI_status.timer = -1;
        return 1;
    }
    return 0;
}
