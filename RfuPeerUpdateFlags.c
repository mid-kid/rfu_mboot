#include <stddef.h>
#include <Agb.h>

#include "RfuPeer.h"
#include "rfuLinkStatus.h"
extern struct RfuPeer RfuPeers[4];
extern struct rfuLinkStatus rfuLinkStatus;
extern void RfuPeerUpdate(u8 Peer, u8 Recv, struct RfuPeerSub *Sub);

u32 RfuPeerUpdateFlags(u8 Flags, u8 Peer)
{
    struct RfuPeerSub *sub;

    u32 ret;
    u16 x;
    u16 y;

    if (Peer >= 4) return 0x600;

    ret = 0x800;
    if (Flags & 0xc) {
        for (x = 0; x < 2; x++) {
            sub = NULL;
            if (x == 0 && Flags & 0x4) {
                sub = &RfuPeers[Peer].sub[0];
                rfuLinkStatus.unk_04 &= ~(1 << Peer);
            } else if (x != 0 && Flags & 0x8) {
                sub = &RfuPeers[Peer].sub[1];
                rfuLinkStatus.unk_05 &= ~(1 << Peer);
            }

            if (sub != NULL) {
                if (sub->unk_01[0] & 0x8000 ||
                        (sub->unk_01[0] == 0x49 && sub->unk_04 != 0x405)) {
                    RfuPeerUpdate(Peer, x, sub);
                    for (y = 0; y < 4; y++) {
                        if (sub->unk_05 & 1 << y) sub->unk_01[1] = 0;
                    }
                }

                CpuClear(0, sub, 52, 16);
            }
        }
        ret = 0;
    }

    return ret;
}
