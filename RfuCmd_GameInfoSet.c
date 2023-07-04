#if 1
__asm__("
.section .text
.global RfuCmd_GameInfoSet
.type RfuCmd_GameInfoSet, function
.thumb_func
RfuCmd_GameInfoSet:
.2byte 0xb530,0x1c04,0x1c0d,0xf7ff,0xfea1,0x4809,0x4909,0xc002,0x1c02,0x8820,0x8010,0x3202,0x3402,0x210d,0x7820,0x7010,0x3201,0x3401,0x3901,0x2900,0xdaf8,0x4b03,0x2100,0xe00b,0x5cc0,0x0300,0x0616,0x9966,0x5ca0,0x0300,0x7010,0x3201,0x3501,0x3101,0x2907,0xdc0a,0x7828,0x2800,0xd1f6,0x2907,0xdc05,0x2000,0x7010,0x3201,0x3101,0x2907,0xddfa,0x2006,0x7218,0xf7ff,0xfeaf,0x0400,0x0c00,0x2801,0xd008,0x4803,0x2100,0xf7ff,0xff0b,0x0400,0x0c00,0xe002,0x0096,0x9966,0x2005,0xbc30,0xbc02,0x4708
.size RfuCmd_GameInfoSet, .-RfuCmd_GameInfoSet
");
#else

#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 RfuBufSend[0x120];
extern struct Rfu Rfu;

u16 RfuCmd_GameInfoSet(char *GameData, char *UserName)
{
    u8 *dst;
    int x;

    RfuCmdReset();

    dst = RfuBufSend;
    *((u32 *)dst)++ = 0x99660616;
    *((u16 *)dst)++ = *((u16 *)GameData)++;

    for (x = 13; x >= 0; x--) *dst++ = *GameData++;

    x = 0;
    for (;;) {
        if (!*UserName) {
            for (;;) {
                if (x < 8) break;
                *dst++ = '\0';
                x++;
            }
            break;
        }
        if (x >= 8) break;
        *dst++ = *UserName++;
        x++;
    }

    Rfu.cmdSize = 6;
    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x99660096, FALSE);
    }
}

#endif
