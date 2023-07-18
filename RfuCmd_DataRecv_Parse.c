#include <Agb.h>

#include "rfuStatic.h"
extern u16 RfuDataRecvParse(u32 unused, u8 *Srcp, u16 Size);
extern struct rfuFixed {
    u8 *recv;
    u8 *send;
} rfuFixed;
extern struct rfuStatic rfuStatic;

void RfuCmd_DataRecv_Parse(void)
{
    u16 done;
    u16 size;
    u8 *Srcp;

    size = *(u16 *)(rfuFixed.recv + 4) & 0x7f;
    Srcp = rfuFixed.recv + 8;

    if (size == 0) rfuStatic.unk_05 = 0xf;

    while (size != 0) {
        done = RfuDataRecvParse(0, Srcp, (u16)size);
        Srcp += done;
        size -= done;
        if (size & 0x8000) break;
    }
}
