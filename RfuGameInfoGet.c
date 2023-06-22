#include <Agb.h>

extern u16 RfuCmd_GameInfoGet(void);
extern void RfuCmd_GetGameInfo_Parse(void);

u16 RfuGameInfoGet(void)
{
    u16 ret;

    ret = RfuCmd_GameInfoGet();
    if (ret == 0) RfuCmd_GetGameInfo_Parse();
    return ret;
}
