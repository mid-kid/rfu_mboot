#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdInit(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 RfuBufSend[0x120];
extern struct Rfu Rfu;

u16 RfuCmd_0x30(u8 param_1)
{
    RfuCmdInit();
    *(u32 *)(RfuBufSend + 0) = 0x99660130;
    *(u32 *)(RfuBufSend + 4) = param_1;
    Rfu.cmdSize = 1;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x996600b0, FALSE);
    }
}
