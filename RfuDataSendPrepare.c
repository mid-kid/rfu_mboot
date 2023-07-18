#include <Agb.h>

#include "RfuPeer.h"
#include "rfuLinkStatus.h"
extern u16 RfuDataSendPreparePeer(u8 Peer, u8 **Destp, struct RfuPeerSub *PeerSub);
extern u8 RfuDataSendBuf[];
extern struct RfuPeer RfuPeers[4];
extern struct rfuLinkStatus rfuLinkStatus;

u32 RfuDataSendPrepare(void)
{
    u8 x;
    u8 *data;
    u32 peersize;
    u32 size;

    size = 0;
    data = RfuDataSendBuf + 4;

    for (x = 0; x < 4; x++) {
        peersize = 0;

        if ((RfuPeers[x].sub[0].unk_01[0] & 0x8000) != 0) {
            peersize += RfuDataSendPreparePeer(x, &data, &RfuPeers[x].sub[0]);
        }

        if ((RfuPeers[x].sub[1].unk_01[0] & 0x8000) != 0) {
            peersize += RfuDataSendPreparePeer(x, &data, &RfuPeers[x].sub[1]);
        }

        if (peersize != 0) {
            if (rfuLinkStatus.mode == 1) {
                size += peersize;
            } else {
                size |= peersize << (x * 5 + 8);
            }
        }
    }

    if (size != 0) {
        while ((u32)data & 3) *data++ = 0;
        *(u32 *)RfuDataSendBuf = size;

        if (rfuLinkStatus.mode == 0) {
            size = data - 4 - RfuDataSendBuf;
        }
    }
    return size;
}
