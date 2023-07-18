#include <Agb.h>

extern u16 STWI_send_CP_EndREQ(void);

u16 RfuConnectReady(void)
{
    return STWI_send_CP_EndREQ();
}
