#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;

u32 Sio32VBlank(void)
{
    if (STWI_status.modeMaster == TRUE) return 0;

    if (STWI_status.unk_10 != (u8)-1) STWI_status.unk_10++;

    if (STWI_status.unk_10 == 6) {
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
        STWI_status.unk_10 = -1;
        return 1;
    }
    return 0;
}
