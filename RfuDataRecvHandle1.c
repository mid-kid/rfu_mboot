#include <Agb.h>

#include "RfuPeer.h"
#include "MbootTmp.h"
extern struct RfuPeer RfuPeers[4];
extern struct MbootTmp MbootTmp;

void RfuDataRecvHandle1(u8 param_1, u8 param_2, u8 *param_3)
{
    u8 x;
    struct RfuPeerSub *sub;
    u16 save_unk_01;
    u8 save_unk_12;
    int diff;

    sub = &RfuPeers[param_1].sub[0];
    save_unk_01 = sub->unk_01[0];
    save_unk_12 = sub->unk_12[param_3[4]];

    if ((param_3[2] == 2 && sub->unk_01[0] == 0x8022) ||
            (param_3[2] == 1 && sub->unk_01[0] == 0x8021) ||
            (param_3[2] == 3 && sub->unk_01[0] == 0x8023)) {
        if (sub->unk_12[param_3[4]] == param_3[5]) {
            sub->unk_06[param_3[4]] |= 1 << param_2;
        }
    }

    if ((sub->unk_06[param_3[4]] & sub->unk_05) == sub->unk_05) {
        sub->unk_12[param_3[4]] = (sub->unk_12[param_3[4]] + 1) & 3;
        sub->unk_06[param_3[4]] = 0;

        if (sub->unk_01[0] == 0x8021 ||
                sub->unk_01[0] == 0x8022) {
            if (sub->unk_01[0] == 0x8021) {
                sub->unk_02[param_3[4]] += sub->unk_21;
            } else {
                sub->unk_02[param_3[4]] += sub->unk_21 * 4;
            }
            diff = sub->unk_03 - sub->unk_21;
            sub->unk_03 = diff;
            if (sub->unk_03 == 0 || diff < 0) {
                sub->unk_11 = 0;
                if (sub->unk_01[0] == 0x8021) {
                    for (x = 0; x < 4; x++) {
                        sub->unk_12[x] = 1;
                        sub->unk_02[x] = &sub->unk_18[sub->unk_21 * x];
                    }
                    sub->unk_03 = sub->unk_22;
                    sub->unk_01[0] = 0x8022;
                } else {
                    sub->unk_12[0] = 0;
                    sub->unk_03 = 0;
                    sub->unk_01[0] = 0x8023;
                }
            }
        } else if (sub->unk_01[0] == 0x8023) {
            sub->unk_01[0] = 0x8020;
        }
    }

    if (sub->unk_01[0] != save_unk_01 ||
            sub->unk_12[param_3[4]] != save_unk_12 ||
            sub->unk_06[param_3[4]] & 1 << param_2) {
        MbootTmp.unk_07 |= 1 << param_2;
        RfuPeers[param_2].sub[0].unk_01[1] = 0;
    }
}
