#include <Agb.h>

extern u16 MbootBeaconID;
extern u16 rfu_REQ_startConnectParent(u16 BeaconID);

u16 REQ_startConnectParent(void)
{
    return rfu_REQ_startConnectParent(MbootBeaconID);
}
