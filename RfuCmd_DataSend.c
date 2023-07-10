#include <Agb.h>

#include "STWI_status.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 STWI_buffer_send[0x120];
extern struct STWI_status STWI_status;

u16 RfuCmd_DataSend(u8 *Srcp, u8 Size)
{
    u16 CmdSize;

    RfuCmdReset();

    CmdSize = Size / 4;
    if (Size % 4) CmdSize++;
    STWI_status.cmdSize = CmdSize;
    CmdSize <<= 8;

    *(u32 *)(STWI_buffer_send + 0) = 0x99660024 + CmdSize;
    CpuSet(Srcp, STWI_buffer_send + 4, STWI_status.cmdSize | DMA_32BIT_BUS);

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x996600a4, TRUE);
    }
}
