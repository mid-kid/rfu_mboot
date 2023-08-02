#include <Agb.h>

#include "AgbRFU_LL.h"
#include "rfuLinkStatus.h"
#include "rfuStatic.h"

RFU_SLOT_STATUS_NI rfuSlotStatus_NI[4];
RFU_LINK_STATUS rfuLinkStatus;
struct rfuStatic rfuStatic;
struct rfuFixed {
    u8 *recv;
    u8 *send;
    u8 func[0x60];
    u8 buf[280];
} rfuFixed;
