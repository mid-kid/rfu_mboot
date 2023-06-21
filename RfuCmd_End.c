#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdInit(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 RfuBufSend[0x120];
extern struct Rfu Rfu;

u16 RfuCmd_End(void)
{
    RfuCmdInit();
    *(u32 *)(RfuBufSend + 0) = 0x9966003d;
    Rfu.cmdSize = 0;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x996600bd, FALSE);
    }
}
