#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;
extern u8 STWI_buffer_recv[0x120];

u16 STWI_check_Command(u32 Cmd, u8 VarSize)
{
    u32 Head;

    // An error happened locally
    if (STWI_status.error != 0) return STWI_status.error;

    // An error has been received
    if (*(u32 *)STWI_buffer_recv == 0x996601ee) return 4;

    Head = *(u32 *)STWI_buffer_recv;
    if (VarSize == TRUE) Head = Head & 0xffff00ff;
    if (Head == Cmd) return 0;
    return 6;
}
