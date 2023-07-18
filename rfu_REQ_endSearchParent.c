#include <Agb.h>

extern u16 STWI_send_SP_EndREQ(void);
extern void rfu_STC_readParentCandidateList(void);

u16 rfu_REQ_endSearchParent(void)
{
    u16 ret;

    ret = STWI_send_SP_EndREQ();
    if (ret == 0) {
        rfu_STC_readParentCandidateList();
    }
    return ret;
}
