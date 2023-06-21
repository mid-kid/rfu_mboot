#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;
extern u8 RfuBufRecv[0x120];

u16 RfuCmdRecv(u32 Cmd, u8 VarSize)
{
    u32 Head;

    // An error happened locally
    if (Rfu.error != 0) return Rfu.error;

    // An error has been received
    if (*(u32 *)RfuBufRecv == 0x996601ee) return 4;

    Head = *(u32 *)RfuBufRecv;
    if (VarSize == TRUE) Head = Head & 0xffff00ff;
    if (Head == Cmd) return 0;
    return 6;
}
