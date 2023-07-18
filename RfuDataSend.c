#include <Agb.h>

#include "RfuPeer.h"
#include "Mboot.h"
#include "MbootTmp.h"
extern u32 RfuDataSendPrepare(void);
extern u16 STWI_send_DataTxREQ(u8 *Srcp, u8 Size);
extern void RfuPeerUpdate(u8 param_1, u8 param_2, struct RfuPeerSub *param_3);
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern u8 RfuDataSendBuf[];
extern struct RfuPeer RfuPeers[4];

u16 RfuDataSend(void)
{
    u16 res;
    u16 x;
    u32 size;
    struct RfuPeerSub *sub;

    res = 0;
    if (Mboot.mode == (u8)-1) return 0;

    MbootTmp.unk_12 = 0;
    size = RfuDataSendPrepare();
    if (MbootTmp.unk_12 != 0) {
        res = STWI_send_DataTxREQ(RfuDataSendBuf, size + 4);
    }

    if (res == 0) {
        for (x = 0; x < 4; x++) {
            if (RfuPeers[x].sub[0].unk_01[0] != 0x8020) continue;

            sub = &RfuPeers[x].sub[0];

            RfuPeerUpdate(x, FALSE, sub);

            Mboot.unk_04 &= ~sub->unk_05;
            if (sub->unk_20 == 1) Mboot.unk_07 |= 1 << x;
            sub->unk_01[0] = 0x27;
        }
    }
    return res;
}
