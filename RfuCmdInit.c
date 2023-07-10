#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;
void RfuCmdReset(void);

u32 RfuCmdInit(void)
{
    RfuCmdReset();
    STWI_status.unk_09 = 8;
    STWI_status.unk_12 = 1;
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE |= 0x81;
    *(vu16 *)REG_IME = 1;
    return 0;
}
