#include <Agb.h>

extern u16 STWI_send_SP_PollingREQ(void);
extern void RfuCmd_GameInfoGet_Parse(void);

u16 RfuGameInfoGet(void)
{
    u16 ret;

    ret = STWI_send_SP_PollingREQ();
    if (ret == 0) RfuCmd_GameInfoGet_Parse();
    return ret;
}
