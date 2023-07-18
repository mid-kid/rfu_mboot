#include <Agb.h>

extern u16 STWI_send_SP_StartREQ(void);

u16 rfu_REQ_startSearchParent(void)
{
    return STWI_send_SP_StartREQ();
}
