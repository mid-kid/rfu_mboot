#include <Agb.h>

extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;

void RfuCmd_ConnectCheck_Parse(u8 *Busy, u8 *PlayerNum, u16 *ID)
{
    u8 *data;

    data = RfuBuf.recv;
    *ID = *(u16 *)(data + 4);
    *PlayerNum = data[6];
    *Busy = data[7];
}
