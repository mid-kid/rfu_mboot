#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
#include "RfuPeer.h"
extern u16 STWI_send_DataRxREQ(void);
extern void RfuCmd_DataRecv_Parse(void);
extern void RfuPeerUpdate(u8 param_1, u8 param_2, struct RfuPeerSub *param_3);
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern struct RfuPeer RfuPeers[4];
extern struct rfuFixed {
    u8 *recv;
    u8 *send;
} rfuFixed;

u32 RfuDataRecv(void)
{
    u16 res;
    u8 x;
    struct RfuPeerSub *sub;

    if (rfuLinkStatus.mode == (u8)-1) return 0;

    rfuStatic.unk_10 = rfuLinkStatus.unk_04 | rfuLinkStatus.unk_05 | rfuLinkStatus.unk_06;
    res = STWI_send_DataRxREQ();
    if (res == 0 && rfuFixed.recv[1] != 0) {
        rfuStatic.unk_05 = 0;
        RfuCmd_DataRecv_Parse();

        for (x = 0; x < 4; x++) {
            if (RfuPeers[x].sub[1].unk_01[0] != 0x8043) continue;
            if (rfuStatic.unk_05 & (1 << x)) continue;

            sub = &RfuPeers[x].sub[1];

            if (sub->unk_20 == 1) rfuLinkStatus.unk_07 |= 1 << x;
            RfuPeerUpdate(x, TRUE, sub);
            rfuLinkStatus.unk_05 &= ~sub->unk_05;
            sub->unk_01[0] = 0x47;
        }
    }
    return res;
}
