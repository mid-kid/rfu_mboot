#include <Agb.h>

#include "rfuLinkStatus.h"
extern struct rfuLinkStatus rfuLinkStatus;

void RfuDisconnect(u8 Peer, u8 Clear)
{
    u8 bit;

    bit = 1 << Peer;
    if (rfuLinkStatus.peersConn & bit) {
        if (rfuLinkStatus.unk_01) rfuLinkStatus.unk_01--;
    }

    rfuLinkStatus.peersConn &= ~bit;
    rfuLinkStatus.peersSeen |= bit;
    if (rfuLinkStatus.mode == 0 && rfuLinkStatus.peersConn == 0) {
        rfuLinkStatus.mode = -1;
    }

    if (Clear) CpuClear(0, rfuLinkStatus.games + Peer, sizeof(struct GameInfo), 16);
}
