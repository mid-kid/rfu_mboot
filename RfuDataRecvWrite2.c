#include <stddef.h>
#include <Agb.h>

#include "RfuPeer.h"
#include "Mboot.h"
extern struct Mboot Mboot;
extern struct RfuPeer RfuPeers[4];

u16 RfuDataRecvWrite2(u8 Peer, struct RfuPeerSub *Sub)
{
    u8 x;

    if (Sub->unk_20 == 1) {
        Sub->unk_02[0] = (u8 *)&Mboot.games[Peer].gameID;
    } else {
        if (RfuPeers[Peer].mbootDest == NULL) {
            Mboot.unk_05 &= ~(1 << Peer);
            Sub->unk_04 = 0x401;
            return 0x49;
        }
        if (Sub->unk_22 > RfuPeers[Peer].mbootSize) {
            Mboot.unk_05 &= ~(1 << Peer);
            Sub->unk_04 = 0x402;
            return 0x49;
        }
        Sub->unk_02[0] = RfuPeers[Peer].mbootDest;
    }

    for (x = 0; x < 4; x++) {
        Sub->unk_12[x] = 0;
        Sub->unk_02[x] = Sub->unk_02[0] + Sub->unk_21 * x;
    }
    Sub->unk_03 = Sub->unk_22;
    return 0x8042;
}
