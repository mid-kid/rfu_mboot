#include <Agb.h>

#include "RfuPeer.h"
#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct RfuPeer RfuPeers[4];
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;

u32 MbootReset(void)
{
    u16 x;

    for (x = 0; x < 4; x++) {
        CpuArrayClear(0, RfuPeers[x].sub, 16);
        rfuLinkStatus.unk_10[x] = 0x10;
    }

    rfuLinkStatus.unk_0f = 0x57;
    rfuLinkStatus.unk_04 = 0;
    rfuLinkStatus.unk_05 = 0;
    rfuLinkStatus.unk_06 = 0;
    rfuStatic.unk_06 = 0;
    rfuStatic.unk_07 = 0;

    return 0;
}
