#include <Agb.h>

extern u16 RfuCmd_DiscoveryResults(void);
extern void RfuCmd_GetGameInfo_Parse(void);

u16 RfuDiscoveryResults(void)
{
    u16 ret;

    ret = RfuCmd_DiscoveryResults();
    if (ret == 0) {
        RfuCmd_GetGameInfo_Parse();
    }
    return ret;
}
