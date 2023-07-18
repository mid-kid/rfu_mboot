#include <Agb.h>
#include <stddef.h>

#include "rfuLinkStatus.h"
#include "RfuPeer.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
    u8 func[1];
} RfuBuf;
extern u8 STWI_buffer_recv[0x120];
extern struct RfuPeer RfuPeers[4];
extern void RfuCmdInit(void);
extern void rfu_STC_clearAPIVariables(void);
extern void RfuMemcpy(u32 *Src, u32 *Dst, int Size);
extern void RfuMemcpyEnd();

void RfuInit(void)
{
    u16 peer;
    u16 size;
    u16 *src;
    u16 *dst;

    RfuCmdInit();
    rfu_STC_clearAPIVariables();
    rfuLinkStatus.unk_09 = 0;
    RfuBuf.recv = STWI_buffer_recv;

    for (peer = 0; peer < 4; peer++) {
        RfuPeers[peer].mbootDest = NULL;
        RfuPeers[peer].mbootSize = 0;
    }

    src = (u16 *)((u32)RfuMemcpy & ~1);
    dst = (u16 *)RfuBuf.func;
    size = (u16 *)RfuMemcpyEnd - (u16 *)RfuMemcpy;
    while (--size != (u16)-1) {
        *dst++ = *src++;
    }

    RfuBuf.send = RfuBuf.func + 1;
}
