#include <Agb.h>

extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern u16 STWI_send_SystemStatusREQ(void);

u32 Rfu0x13(u8 *Recv)
{
    u16 ret;

    ret = STWI_send_SystemStatusREQ();
    if (ret == 0) {
        *Recv = RfuBuf.recv[7];
    } else {
        *Recv = 0xff;
    }

    return ret;
}
