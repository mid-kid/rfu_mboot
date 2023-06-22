#include <Agb.h>

extern u16 RfuCmd_ConnectReady(void);

u16 RfuConnectReady(void)
{
    return RfuCmd_ConnectReady();
}
