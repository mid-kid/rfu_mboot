#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern void RfuResetSub(u8 param_1);

void RfuReset(void)
{
    u16 ime;

    ime = *(vu16 *)REG_IME;
    *(vu16 *)REG_IME = 0;

    if (rfuLinkStatus.unk_04 != 0) RfuResetSub(0);
    if (rfuLinkStatus.unk_05 != 0) RfuResetSub(1);
    rfuStatic.unk_07 = 0;
    rfuStatic.unk_06 = 0;

    *(vu16 *)REG_IME = ime;
}
