#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
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

    if (rfuLinkStatus.mode == (u8)-1) return 0;

    if (!rfuStatic.unk_04) {
        rfuStatic.unk_04 = rfuLinkStatus.unk_09;
        mode = 1;
    }

    if (RfuBuf.recv[0] == 0x29) {
        *PeersLost = RfuBuf.recv[4];
        *Connected = RfuBuf.recv[5];
        if (*Connected == 1) *PeersLost = rfuLinkStatus.peersConn;
        mode = 2;
    }

    if (mode) {
        ret = STWI_send_LinkStatusREQ();
        if (ret == 0) {
            puVar6 = RfuBuf.recv + 4;
            for (x = 0; x < 4; x++) {
                bit = 1 << x;

                if (rfuLinkStatus.peersConn & bit && !*puVar6 && mode == 1) {
                    *PeersLost |= bit;
                    *Connected = 1;
                }

                if (rfuLinkStatus.peersSeen & bit && *puVar6 && rfuLinkStatus.mode == 1) {
                    *PeersSeen |= bit;
                    rfuLinkStatus.peersConn |= bit;
                    rfuLinkStatus.peersSeen &= ~bit;
                    rfuLinkStatus.unk_01++;
                }

                rfuLinkStatus.unk_0a[x] = *puVar6;
                puVar6++;
            }
        }

        for (x = 0; x < 4; x++) {
            if (rfuLinkStatus.peersConn & (1 << x) && *PeersLost & (1 << x)) {
                RfuDisconnect(x, FALSE);
            }
        }
    }

    return ret;
}
