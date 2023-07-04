#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;
void RfuCmdReset(void);

u32 RfuCmdInit(void)
{
    RfuCmdReset();
    Rfu.unk_09 = 8;
    Rfu.unk_12 = 1;
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE |= 0x81;
    *(vu16 *)REG_IME = 1;
    return 0;
}
