#include <Agb.h>

extern u16 STWI_send_ResetREQ(void);
extern void rfu_STC_clearAPIVariables(void);

u16 rfu_REQ_reset(void)
{
    u16 ret;

    ret = STWI_send_ResetREQ();
    if (ret == 0) rfu_STC_clearAPIVariables();
    return ret;
}
