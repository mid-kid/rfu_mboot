#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;

u32 Sio32VBlank(void)
{
    if (Rfu.modeMaster == TRUE) return 0;

    if (Rfu.unk_10 != (u8)-1) Rfu.unk_10++;

    if (Rfu.unk_10 == 6) {
        Rfu.state = 5;
        Rfu.cmdHeader = 0;
        Rfu.cmdSize = 0;
        Rfu.field3_0x9 = 0;
        Rfu.error = 0;
        Rfu.unk_07 = 0;
        Rfu.unk_08 = 0;
        *(vu32 *)REG_SIODATA32 = 0x80000000;
        *(vu16 *)REG_SIOCNT = 0;
        *(vu16 *)REG_SIOCNT = 0x5003;
        *(vu16 *)REG_SIOCNT = 0x5082;
        Rfu.unk_10 = -1;
        return 1;
    }
    return 0;
}
