#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern void STWI_intr_vblank(void);

void RfuVBlank(void)
{
    if (rfuLinkStatus.mode != (u8)-1) {
        if (rfuStatic.unk_04) rfuStatic.unk_04--;
    }
    STWI_intr_vblank();
}
