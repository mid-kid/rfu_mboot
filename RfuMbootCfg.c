#include <Agb.h>

#include "RfuPeer.h"
extern struct RfuPeer RfuPeers[4];

u32 RfuMbootCfg(u8 param_1, u8 Peer, void *Dest, u32 Size)
{
    if (Peer >= 4) {
        return 0x600;
    }

    if (param_1 & 0x20) {
        RfuPeers[Peer].mbootDest = Dest;
        RfuPeers[Peer].mbootSize = Size;
    }
    return 0;
}
