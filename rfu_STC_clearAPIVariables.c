#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern u32 rfu_clearAllSlot(void);

void rfu_STC_clearAPIVariables(void)
{
    u8 save;

    CpuClear(0, &rfuStatic, 16, 16);
    save = rfuLinkStatus.unk_09;
    CpuClear(0, &rfuLinkStatus, 0x5a*2, 16);
    rfuStatic.unk_04 = save;
    rfuLinkStatus.unk_09 = save;
    rfuLinkStatus.mode = 0xff;
    rfu_clearAllSlot();
}
