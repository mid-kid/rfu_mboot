#include <Agb.h>

extern u16 RfuConnectCheck(u8 *param_1, u8 *GameIndex);
extern u8 MbootPeer;

u16 SearchProc_ConnectCheck(void)
{
    u16 ret;
    u8 temp[4];

    ret = RfuConnectCheck(temp, &MbootPeer);
    if (ret == 0 && temp[0] != 0) {
        ret = temp[0] | 0x8000;
    }
    return ret;
}
