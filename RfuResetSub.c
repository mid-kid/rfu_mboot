#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
#include "RfuPeer.h"
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern struct RfuPeer RfuPeers[4];

void RfuResetSub(u8 param_1)
{
    u8 x;
    u8 bit;
    u8 *puVar3;
    u8 *puVar4;

    if (param_1 == 0) {
        puVar4 = &Mboot.unk_04;
        puVar3 = &MbootTmp.unk_07;
    } else {
        puVar4 = &Mboot.unk_05;
        puVar3 = &MbootTmp.unk_06;
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
