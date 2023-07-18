#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;

void RfuSetUnk04(u8 val)
{
    struct rfuLinkStatus *ptr = &rfuLinkStatus;
    rfuStatic.unk_04 = val;
    ptr->unk_09 = val;
}
