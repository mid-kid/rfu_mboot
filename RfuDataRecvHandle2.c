#include <Agb.h>

#include "RfuPeer.h"
#include "MbootTmp.h"
extern struct RfuPeer RfuPeers[4];
extern struct MbootTmp MbootTmp;
extern u32 FUN_03003ce0(u8 Peer, struct RfuPeerSub *Sub);
extern u16 FUN_03003d38(u8 Peer, struct RfuPeerSub *Sub);
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;

void RfuDataRecvHandle2(u8 Peer, u8 *param_2, u8 *param_3)
{
    u8 cont;
    struct RfuPeerSub *sub;
    u16 save_unk_01;
    u8 save_unk_12;
    u8 *ptr_unk_11;

    cont = FALSE;
    sub = &RfuPeers[Peer].sub[1];
    save_unk_01 = sub->unk_01[0];
    save_unk_12 = sub->unk_12[param_2[4]];

    if (sub->unk_01[0] == 0x46 || sub->unk_01[0] == 0x47) return;

    if (param_2[2] == 3) {
        MbootTmp.unk_05 |= 1 << Peer;
        ptr_unk_11 = &sub->unk_11;
        if (sub->unk_01[0] == 0x8042) {
            *ptr_unk_11 = 0;
            sub->unk_12[0] = 0;
            sub->unk_01[0] = 0x8043;
        }
    } else if (param_2[2] == 2) {
        if (sub->unk_01[0] == 0x8041 && sub->unk_03 == 0) {
            sub->unk_01[0] = FUN_03003d38(Peer, sub);
        }
        if (sub->unk_01[0] == 0x8042) {
            cont = TRUE;
        }
    } else if (param_2[2] == 1) {
        if (!(sub->unk_01[0] & 0x8000)) {
            sub->unk_01[0] = FUN_03003ce0(Peer, sub);
        }
        if (sub->unk_01[0] == 0x8041) {
            cont = TRUE;
        }
    }

    if (cont && param_2[5] == ((sub->unk_12[param_2[4]] + 1) & 3)) {
        ((void (*)())RfuBuf.send)(&param_3, &sub->unk_02[param_2[4]],
            *(u16 *)&param_2[6]);
        if (sub->unk_01[0] == 0x8042) {
            sub->unk_02[param_2[4]] += sub->unk_21 * 3;
        }
        sub->unk_03 -= *(u16 *)&param_2[6];
        sub->unk_12[param_2[4]] = param_2[5];
    }
    sub->unk_11 = param_2[4];

    if (sub->unk_01[0] != save_unk_01 ||
            sub->unk_12[param_2[4]] != save_unk_12 ||
            sub->unk_12[param_2[4]] == param_2[5]) {
        MbootTmp.unk_06 |= 1 << Peer;
        sub->unk_01[1] = 0;
    }
}
