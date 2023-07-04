#include <Agb.h>
#include <stddef.h>

#include "Mboot.h"
#include "RfuPeer.h"
extern struct Mboot Mboot;
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
    u8 func[1];
} RfuBuf;
extern u8 RfuBufRecv[0x120];
extern struct RfuPeer RfuPeers[4];
extern void RfuMemcpyFunc();
extern void Sio32_Unk5(void);
extern void Init_Unk292c(void);
extern void RfuMemcpy(u32 *Src, u32 *Dst, int Size);
extern void RfuMemcpyEnd();

void RfuInit(void)
{
    u16 peer;
    u16 size;
    u16 *src;
    u16 *dst;

    Sio32_Unk5();
    Init_Unk292c();
    Mboot.unk_09 = 0;
    RfuBuf.recv = RfuBufRecv;

    for (peer = 0; peer < 4; peer++) {
        RfuPeers[peer].mbootDest = NULL;
        RfuPeers[peer].mbootSize = 0;
    }

    src = (u16 *)((u32)RfuMemcpy & ~1);
    dst = (u16 *)RfuBuf.func;
    size = (u16 *)RfuMemcpyEnd - (u16 *)RfuMemcpy;
    while (--size != 0xffff) {
        *dst++ = *src++;
    }

    RfuBuf.send = RfuBuf.func + 1;
}
