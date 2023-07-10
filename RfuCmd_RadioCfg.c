#include <Agb.h>

#include "STWI_status.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 STWI_buffer_send[0x120];
extern struct STWI_status STWI_status;

u16 RfuCmd_RadioCfg(u16 param_1, u8 param_2, u8 param_3)
{
    RfuCmdReset();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660117;
    *(u8 *)(STWI_buffer_send + 4) = param_3;
    *(u8 *)(STWI_buffer_send + 5) = param_2;
    *(u16 *)(STWI_buffer_send + 6) = param_1;
    STWI_status.cmdSize = 1;

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x99660097, FALSE);
    }
}
