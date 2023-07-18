#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern u16 STWI_send_CP_StartREQ(u16 BeaconID);
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;

u16 rfu_REQ_startConnectParent(u16 BeaconID)
{
    u16 ret;
    u16 x;

    for (x = 0; x < 4 && rfuLinkStatus.games[x].beaconID != BeaconID; x++);
    if (x == 4) return 0x900;

    ret = STWI_send_CP_StartREQ(BeaconID);
    if (ret == 0) {
        rfuStatic.beaconID = BeaconID;
    }
    return ret;
}
