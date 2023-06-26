#if 1
__asm__("
.section .text
.global RfuIntrDataTransfer
.type RfuIntrDataTransfer, function
.thumb_func
RfuIntrDataTransfer:
.2byte 0xb570,0xb081,0x2400,0x480c,0x7004,0x466a,0x3201,0x4668,0x1c11,0xf001,0xffa7,0x4668,0x7800,0x2800,0xd020,0x4a07,0x4807,0x7801,0x00c8,0x1a40,0x0100,0x1880,0x8e80,0x2800,0xd00a,0x4904,0x2001,0xe009,0x5750,0x0300,0x5f00,0x0300,0x5692,0x0300,0x5693,0x0300,0x4903,0x2003,0x7008,0x4903,0x20c1,0x7008,0xe01b,0x0000,0x5693,0x0300,0x5760,0x0300,0x4d0d,0x7828,0x28c5,0xd803,0xf002,0xfc38,0xf002,0xfa94,0x4e0a,0xf002,0xf895,0x0400,0x2800,0xd008,0x1c60,0x0600,0x0e04,0x2c02,0xd1f5,0x2004,0x7030,0x20c1,0x7028,0xb001,0xbc70,0xbc01,0x4700,0x0000,0x5760,0x0300,0x5693,0x0300
.size RfuIntrDataTransfer, .-RfuIntrDataTransfer
");
#else

#include <Agb.h>

#include "RfuPeer.h"
extern u32 RfuCmd_0x11_Call_Unk2d74(u8 *param_1, u8 *param_2, u8 *param_3);
extern u32 RfuDataRecv(void);
extern u32 RfuDataSend(void);
extern u16 RfuWaitDataStartForce(void);
extern u8 SearchMenuErrorTimer;
extern u8 MenuState;
extern u8 SearchMenuErrorMsg;
extern struct RfuPeer RfuPeers[4];
extern u8 MbootPeer;

void RfuIntrDataTransfer(void)
{
    u8 x;
    u8 res;
    u8 res2;
    u8 res3;

    x = 0;
    SearchMenuErrorTimer = 0;

    RfuCmd_0x11_Call_Unk2d74(&res, &res2, &res3);
    if (res != 0) {
        if (RfuPeers[MbootPeer].sub[1].unk_01[0] != 0) {
            SearchMenuErrorMsg = 1;
        } else {
            SearchMenuErrorMsg = 3;
        }
    } else {
        if (MenuState < 0xc6) {
            RfuDataRecv();
            RfuDataSend();
        }

        for (x = 0; x != 2; x++) {
            if (RfuWaitDataStartForce() == 0) return;
        }
        SearchMenuErrorMsg = 4;
    }

    MenuState = 0xc1;
}
#endif
