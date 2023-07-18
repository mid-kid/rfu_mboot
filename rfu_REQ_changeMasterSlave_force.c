#include <Agb.h>

#include "rfuStatic.h"
extern struct rfuStatic rfuStatic;
extern u16 STWI_send_MS_ChangeREQ(void);

u16 rfu_REQ_changeMasterSlave_force(void)
{
    u16 ret = 0;

    if (rfuStatic.unk_02 == 0) {
        ret = STWI_send_MS_ChangeREQ();
    } else {
        rfuStatic.unk_02 = 0;
    }

    return ret;
}
