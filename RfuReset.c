#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern void RfuResetSub(u8 param_1);

void RfuReset(void)
{
    u16 ime;

    ime = *(vu16 *)REG_IME;
    *(vu16 *)REG_IME = 0;

    if (Mboot.unk_04 != 0) RfuResetSub(0);
    if (Mboot.unk_05 != 0) RfuResetSub(1);
    MbootTmp.unk_07 = 0;
    MbootTmp.unk_06 = 0;

    *(vu16 *)REG_IME = ime;
}
