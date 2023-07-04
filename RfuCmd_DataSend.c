#include <Agb.h>

#include "Rfu.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 RfuBufSend[0x120];
extern struct Rfu Rfu;

u16 RfuCmd_DataSend(u8 *Srcp, u8 Size)
{
    u16 CmdSize;

    RfuCmdReset();

    CmdSize = Size / 4;
    if (Size % 4) CmdSize++;
    Rfu.cmdSize = CmdSize;
    CmdSize <<= 8;

    *(u32 *)(RfuBufSend + 0) = 0x99660024 + CmdSize;
    CpuSet(Srcp, RfuBufSend + 4, Rfu.cmdSize | DMA_32BIT_BUS);

    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x996600a4, TRUE);
    }
}
