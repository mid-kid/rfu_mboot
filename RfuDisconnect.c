#include <Agb.h>

#include "Mboot.h"
extern struct Mboot Mboot;

void RfuDisconnect(u8 Peer, u8 Clear)
{
    u8 bit;

    bit = 1 << Peer;
    if (Mboot.peersConn & bit && Mboot.unk_01 != 0) {
        Mboot.unk_01--;
    }

    Mboot.peersConn &= ~bit;
    Mboot.peersSeen |= bit;
    if (Mboot.mode == 0 && Mboot.peersConn == 0) {
        Mboot.mode = -1;
    }

    if (Clear) CpuClear(0, Mboot.games + Peer, sizeof(struct GameInfo), 16);
}
