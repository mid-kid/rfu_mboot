#include <Agb.h>

#include "RfuPeer.h"
#include "rfuLinkStatus.h"
extern struct rfuLinkStatus rfuLinkStatus;

u32 RfuDataRecvWrite1(u8 Peer, struct RfuPeerSub *Sub)
{
    u32 max;
    u8 *ptr;

    max = 2;
    ptr = &rfuLinkStatus.unk_10[Peer];

    if (*ptr < max) {
        Sub->unk_04 = 0x405;
        return 0x49;
    }
    Sub->unk_04 = 0;

    *ptr -= max;
    Sub->unk_05 = 1 << Peer;
    Sub->unk_02[0] = &Sub->unk_20;
    Sub->unk_03 = 7;
    Sub->unk_10 = 1;
    Sub->unk_21 = 0;
    rfuLinkStatus.unk_05 |= 1 << Peer;
    return 0x8041;
}
