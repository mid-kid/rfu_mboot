#include <Agb.h>

extern struct rfuFixed {
    u8 *recv;
    u8 *send;
} rfuFixed;

void rfu_getConnectParentStatus(u8 *Busy, u8 *PlayerNum, u16 *ID)
{
    u8 *data;

    data = rfuFixed.recv;
    *ID = *(u16 *)(data + 4);
    *PlayerNum = data[6];
    *Busy = data[7];
}
