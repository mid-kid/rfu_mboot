#include <Agb.h>

extern u16 rfu_REQ_pollConnectParent(u8 *param_1, u8 *GameIndex);
extern u8 MbootPeer;

u16 REQ_pollConnectParent(void)
{
    u16 ret;
    u8 temp[4];

    ret = rfu_REQ_pollConnectParent(temp, &MbootPeer);
    if (ret == 0 && temp[0] != 0) {
        ret = temp[0] | 0x8000;
    }
    return ret;
}
