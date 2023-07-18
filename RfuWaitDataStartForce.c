#include <Agb.h>

#include "MbootTmp.h"
extern struct MbootTmp MbootTmp;
extern u16 STWI_send_MS_ChangeREQ(void);

u16 RfuWaitDataStartForce(void)
{
    u16 ret = 0;

    if (MbootTmp.unk_02 == 0) {
        ret = STWI_send_MS_ChangeREQ();
    } else {
        MbootTmp.unk_02 = 0;
    }

    return ret;
}
