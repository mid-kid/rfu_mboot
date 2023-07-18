#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern u16 STWI_send_LinkStatusREQ(void);
extern void RfuDisconnect(u8 Peer, u8 param_2);

u32 RfuStatus(u8 *PeersLost, u8 *Connected, u8 *PeersSeen)
{
    u8 *puVar6;

    u32 ret;
    u8 mode;
    u8 x;
    u8 bit;

    mode = 0;
    ret = 0;
    *PeersLost = 0;
    *Connected = 0;
    *PeersSeen = 0;

    if (Mboot.mode == (u8)-1) return 0;

    if (!MbootTmp.unk_04) {
        MbootTmp.unk_04 = Mboot.unk_09;
        mode = 1;
    }

    if (RfuBuf.recv[0] == 0x29) {
        *PeersLost = RfuBuf.recv[4];
        *Connected = RfuBuf.recv[5];
        if (*Connected == 1) *PeersLost = Mboot.peersConn;
        mode = 2;
    }

    if (mode) {
        ret = STWI_send_LinkStatusREQ();
        if (ret == 0) {
            puVar6 = RfuBuf.recv + 4;
            for (x = 0; x < 4; x++) {
                bit = 1 << x;

                if (Mboot.peersConn & bit && !*puVar6 && mode == 1) {
                    *PeersLost |= bit;
                    *Connected = 1;
                }

                if (Mboot.peersSeen & bit && *puVar6 && Mboot.mode == 1) {
                    *PeersSeen |= bit;
                    Mboot.peersConn |= bit;
                    Mboot.peersSeen &= ~bit;
                    Mboot.unk_01++;
                }

                Mboot.unk_0a[x] = *puVar6;
                puVar6++;
            }
        }

        for (x = 0; x < 4; x++) {
            if (Mboot.peersConn & (1 << x) && *PeersLost & (1 << x)) {
                RfuDisconnect(x, FALSE);
            }
        }
    }

    return ret;
}
