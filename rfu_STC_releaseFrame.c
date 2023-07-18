#include <Agb.h>

#include "rfuLinkStatus.h"
#include "RfuPeer.h"
extern struct rfuLinkStatus rfuLinkStatus;

void rfu_STC_releaseFrame(u8 Peer, u8 Recv, struct RfuPeerSub *Sub)
{
    if (!Recv) rfuLinkStatus.unk_10[Peer] += Sub->unk_21;
    rfuLinkStatus.unk_10[Peer] += 2;
}
