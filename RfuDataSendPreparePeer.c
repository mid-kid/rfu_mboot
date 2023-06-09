#include <Agb.h>

#include "RfuPeer.h"
#include "Mboot.h"
#include "MbootTmp.h"
extern struct Mboot Mboot;
extern u8 RfuEncTable[2][16];
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern struct MbootTmp MbootTmp;

u16 RfuDataSendPreparePeer(u8 Peer, u8 **Destp, struct RfuPeerSub *PeerSub)
{
    u16 x;
    u16 size;
    u32 flags;
    u8 *enc;
    u8 *ptr;
    u8 *flags_ptr;
    u8 *temp_ptr;

    enc = RfuEncTable[Mboot.mode];

    if (PeerSub->unk_01[0] == 0x8022) {
        ptr = &PeerSub->unk_11;
        while (PeerSub->unk_02[*ptr] >= PeerSub->unk_18 + PeerSub->unk_22) {
            if (++(*ptr) == 4) PeerSub->unk_11 = 0;
        }
    }

    if (PeerSub->unk_01[0] & 0x40) {
        size = 0;
    } else if (PeerSub->unk_01[0] == 0x8022) {
        if (PeerSub->unk_02[PeerSub->unk_11] + PeerSub->unk_21 >
                PeerSub->unk_18 + PeerSub->unk_22) {
            size = (PeerSub->unk_18 + PeerSub->unk_22) -
                PeerSub->unk_02[PeerSub->unk_11];
        } else {
            size = PeerSub->unk_21;
        }
    } else {
        if (PeerSub->unk_03 >= PeerSub->unk_21) {
            size = PeerSub->unk_21;
        } else {
            size = PeerSub->unk_03;
        }
    }

    flags =
        (PeerSub->unk_01[0] & 0xf) << enc[3] |
        PeerSub->unk_10 << enc[4] |
        PeerSub->unk_11 << enc[5] |
        PeerSub->unk_12[PeerSub->unk_11] << enc[6] |
        size;

    if (Mboot.mode == 1) {
        flags |= PeerSub->unk_05 << 18;
    }

    flags_ptr = (u8 *)&flags;
    for (x = 0; x < enc[0]; x++) *(*Destp)++ = *flags_ptr++;

    if (size != 0) {
        temp_ptr = PeerSub->unk_02[PeerSub->unk_11];
        ((void (*)())RfuBuf.send)(&temp_ptr, Destp, size);
    }

    if (PeerSub->unk_01[0] == 0x8022) {
        ptr = &PeerSub->unk_11;
        if (++(*ptr) == 4) PeerSub->unk_11 = 0;
    }

    MbootTmp.unk_12 |= 1 << Peer;

    return size + enc[0];
}
