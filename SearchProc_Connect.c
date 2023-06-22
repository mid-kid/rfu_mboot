#include <Agb.h>

extern u16 MbootBeaconID;
extern u16 RfuConnect(u16 BeaconID);

u16 SearchProc_Connect(void)
{
    return RfuConnect(MbootBeaconID);
}
