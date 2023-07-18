#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern u16 STWI_send_CP_StartREQ(u16 BeaconID);
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;

u16 RfuConnect(u16 BeaconID)
{
    u16 ret;
    u16 x;

    for (x = 0; x < 4 && Mboot.games[x].beaconID != BeaconID; x++);
    if (x == 4) return 0x900;

    ret = STWI_send_CP_StartREQ(BeaconID);
    if (ret == 0) {
        MbootTmp.beaconID = BeaconID;
    }
    return ret;
}
