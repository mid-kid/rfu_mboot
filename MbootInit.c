#include <Agb.h>

#include "RfuPeer.h"
#include "Mboot.h"
#include "MbootTmp.h"
extern struct RfuPeer RfuPeers[4];
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;

u32 MbootInit(void)
{
    u16 x;

    for (x = 0; x < 4; x++) {
        CpuArrayClear(0, RfuPeers[x].sub, 16);
        Mboot.unk_10[x] = 0x10;
    }

    Mboot.unk_0f = 0x57;
    Mboot.unk_04 = 0;
    Mboot.unk_05 = 0;
    Mboot.unk_06 = 0;
    MbootTmp.unk_06 = 0;
    MbootTmp.unk_07 = 0;

    return 0;
}
