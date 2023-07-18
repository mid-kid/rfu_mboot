#include <Agb.h>

extern u16 STWI_send_SP_EndREQ(void);
extern void RfuCmd_GameInfoGet_Parse(void);

u16 RfuDiscoveryResults(void)
{
    u16 ret;

    ret = STWI_send_SP_EndREQ();
    if (ret == 0) {
        RfuCmd_GameInfoGet_Parse();
    }
    return ret;
}
