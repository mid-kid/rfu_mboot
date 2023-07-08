#include <Agb.h>

extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern u16 RfuCmd_0x13(void);

u32 Rfu0x13(u8 *Recv)
{
    u16 ret;

    ret = RfuCmd_0x13();
    if (ret == 0) {
        *Recv = RfuBuf.recv[7];
    } else {
        *Recv = 0xff;
    }

    return ret;
}
