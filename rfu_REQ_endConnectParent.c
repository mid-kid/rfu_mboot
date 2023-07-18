#include <Agb.h>

extern u16 STWI_send_CP_EndREQ(void);

u16 rfu_REQ_endConnectParent(void)
{
    return STWI_send_CP_EndREQ();
}
