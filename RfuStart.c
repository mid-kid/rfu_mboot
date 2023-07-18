#include <Agb.h>

extern u16 STWI_send_ResetREQ(void);
extern void MbootInit(void);

u16 RfuStart(void)
{
    u16 ret;

    ret = STWI_send_ResetREQ();
    if (ret == 0) MbootInit();
    return ret;
}
