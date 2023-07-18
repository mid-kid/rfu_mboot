#include <Agb.h>

#include "RfuPeer.h"
#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern u32 rfu_constructSendLLFrame(void);
extern u16 STWI_send_DataTxREQ(u8 *Srcp, u8 Size);
extern void rfu_STC_releaseFrame(u8 param_1, u8 param_2, struct RfuPeerSub *param_3);
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern u8 RfuDataSendBuf[];
extern struct RfuPeer RfuPeers[4];

u16 rfu_REQ_sendData(void)
{
    u16 res;
    u16 x;
    u32 size;
    struct RfuPeerSub *sub;

    res = 0;
    if (rfuLinkStatus.mode == (u8)-1) return 0;

    rfuStatic.unk_12 = 0;
    size = rfu_constructSendLLFrame();
    if (rfuStatic.unk_12 != 0) {
        res = STWI_send_DataTxREQ(RfuDataSendBuf, size + 4);
    }

    if (res == 0) {
        for (x = 0; x < 4; x++) {
            if (RfuPeers[x].sub[0].unk_01[0] != 0x8020) continue;

            sub = &RfuPeers[x].sub[0];

            rfu_STC_releaseFrame(x, FALSE, sub);

            rfuLinkStatus.unk_04 &= ~sub->unk_05;
            if (sub->unk_20 == 1) rfuLinkStatus.unk_07 |= 1 << x;
            sub->unk_01[0] = 0x27;
        }
    }
    return res;
}
