#include <Agb.h>

#include "Mboot.h"
#include "RfuPeer.h"
extern struct Mboot Mboot;

void RfuPeerUpdate(u8 Peer, u8 Recv, struct RfuPeerSub *Sub)
{
    if (!Recv) Mboot.unk_10[Peer] += Sub->unk_21;
    Mboot.unk_10[Peer] += 2;
}
