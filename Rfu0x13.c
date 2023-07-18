#include <Agb.h>

extern struct rfuFixed {
    u8 *recv;
    u8 *send;
} rfuFixed;
extern u16 STWI_send_SystemStatusREQ(void);

u32 Rfu0x13(u8 *Recv)
{
    u16 ret;

    ret = STWI_send_SystemStatusREQ();
    if (ret == 0) {
        *Recv = rfuFixed.recv[7];
    } else {
        *Recv = 0xff;
    }

    return ret;
}
