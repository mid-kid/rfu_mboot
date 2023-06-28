#include <Agb.h>

#include "MbootTmp.h"
extern u16 RfuDataRecvParse(u32 unused, u8 *Srcp, u16 Size);
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern struct MbootTmp MbootTmp;

void RfuCmd_DataRecv_Parse(void)
{
    u16 done;
    u16 size;
    u8 *Srcp;

    size = *(u16 *)(RfuBuf.recv + 4) & 0x7f;
    Srcp = RfuBuf.recv + 8;

    if (size == 0) MbootTmp.unk_05 = 0xf;

    while (size != 0) {
        done = RfuDataRecvParse(0, Srcp, (u16)size);
        Srcp += done;
        size -= done;
        if (size & 0x8000) break;
    }
}
