#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 RfuBufSend[0x120];
extern struct Rfu Rfu;

u16 RfuCmd_DiscoveryStart(void)
{
    RfuCmdReset();
    *(u32 *)(RfuBufSend + 0) = 0x9966001c;
    Rfu.cmdSize = 0;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x9966009c, FALSE);
    }
}
