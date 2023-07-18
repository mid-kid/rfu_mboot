#include <Agb.h>
#include <stddef.h>

#include "rfuLinkStatus.h"
#include "RfuPeer.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuFixed {
    u8 *recv;
    u8 *send;
    u8 func[1];
} rfuFixed;
extern u8 STWI_buffer_recv[0x120];
extern struct RfuPeer RfuPeers[4];
extern void STWI_init_all(void);
extern void rfu_STC_clearAPIVariables(void);
extern void rfu_STC_fastCopy(u32 *Src, u32 *Dst, int Size);
extern void rfu_STC_fastCopy_end();

void rfu_initializeAPI(void)
{
    u16 peer;
    u16 size;
    u16 *src;
    u16 *dst;

    STWI_init_all();
    rfu_STC_clearAPIVariables();
    rfuLinkStatus.unk_09 = 0;
    rfuFixed.recv = STWI_buffer_recv;

    for (peer = 0; peer < 4; peer++) {
        RfuPeers[peer].mbootDest = NULL;
        RfuPeers[peer].mbootSize = 0;
    }

    src = (u16 *)((u32)rfu_STC_fastCopy & ~1);
    dst = (u16 *)rfuFixed.func;
    size = (u16 *)rfu_STC_fastCopy_end - (u16 *)rfu_STC_fastCopy;
    while (--size != (u16)-1) {
        *dst++ = *src++;
    }

    rfuFixed.send = rfuFixed.func + 1;
}
