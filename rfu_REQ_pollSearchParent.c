#include <Agb.h>

extern u16 STWI_send_SP_PollingREQ(void);
extern void rfu_STC_readParentCandidateList(void);

u16 rfu_REQ_pollSearchParent(void)
{
    u16 ret;

    ret = STWI_send_SP_PollingREQ();
    if (ret == 0) rfu_STC_readParentCandidateList();
    return ret;
}
