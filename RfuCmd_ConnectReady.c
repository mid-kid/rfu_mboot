#include <Agb.h>

#include "STWI_status.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 STWI_buffer_send[0x120];
extern struct STWI_status STWI_status;

u16 RfuCmd_ConnectReady(void)
{
    RfuCmdReset();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660021;
    STWI_status.cmdSize = 0;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x996601a1, FALSE);
    }
}
