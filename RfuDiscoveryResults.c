#include <Agb.h>

extern u16 RfuCmd_DiscoveryResults(void);
extern void RfuCmd_GameInfoGet_Parse(void);

u16 RfuDiscoveryResults(void)
{
    u16 ret;

    ret = RfuCmd_DiscoveryResults();
    if (ret == 0) {
        RfuCmd_GameInfoGet_Parse();
    }
    return ret;
}
