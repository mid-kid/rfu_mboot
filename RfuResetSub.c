#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
#include "RfuPeer.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern struct RfuPeer RfuPeers[4];

void RfuResetSub(u8 param_1)
{
    u8 x;
    u8 bit;
    u8 *puVar3;
    u8 *puVar4;

    if (param_1 == 0) {
        puVar4 = &rfuLinkStatus.unk_04;
        puVar3 = &rfuStatic.unk_07;
    } else {
        puVar4 = &rfuLinkStatus.unk_05;
        puVar3 = &rfuStatic.unk_06;
    }

    for (x = 0; x < 4; x++) {
        bit = 0x1 << x;

        if ((*puVar4 & bit) != 0 &&
                (*puVar3 & bit) == 0 &&
                (param_1 != 1 || (RfuPeers[x].sub[1].unk_01[0] != 0x8043))) {
            if (param_1 == 0) {
                RfuPeers[x].sub[0].unk_01[1]++;
            } else {
                RfuPeers[x].sub[1].unk_01[1]++;
            }
        }
    }
}
