#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
#include "RfuPeer.h"
extern u16 RfuCmd_DataRecv(void);
extern void RfuCmd_DataRecv_Parse(void);
extern void FUN_03003044(u8 param_1, u8 param_2, struct RfuPeerSub *param_3);
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern struct RfuPeer RfuPeers[4];
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;

u32 RfuDataRecv(void)
{
    u16 res;
    u8 x;
    struct RfuPeerSub *sub;

    if (Mboot.mode == (u8)-1) return 0;

    MbootTmp.unk_10 = Mboot.unk_04 | Mboot.unk_05 | Mboot.unk_06;
    res = RfuCmd_DataRecv();
    if (res == 0 && RfuBuf.recv[1] != 0) {
        MbootTmp.unk_05 = 0;
        RfuCmd_DataRecv_Parse();

        for (x = 0; x < 4; x++) {
            if (RfuPeers[x].sub[1].unk_01[0] != 0x8043) continue;
            if (MbootTmp.unk_05 & (1 << x)) continue;

            sub = &RfuPeers[x].sub[1];

            if (sub->unk_20 == 1) Mboot.unk_07 |= 1 << x;
            FUN_03003044(x, TRUE, sub);
            Mboot.unk_05 &= ~sub->unk_05;
            sub->unk_01[0] = 0x47;
        }
    }
    return res;
}
