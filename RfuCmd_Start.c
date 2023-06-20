#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdInit(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u32 RfuBufSend[0x48];
extern struct Rfu Rfu;

u16 RfuCmd_Start(void)
{
    RfuCmdInit();
    RfuBufSend[0] = 0x99660010;
    Rfu.field2_0x8 = 0;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x99660090, FALSE);
    }
}
