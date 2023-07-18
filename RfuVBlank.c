#include <Agb.h>

#include "rfuLinkStatus.h"
#include "rfuStatic.h"
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern void Sio32VBlank(void);

void RfuVBlank(void)
{
    if (rfuLinkStatus.mode != (u8)-1) {
        if (rfuStatic.unk_04) rfuStatic.unk_04--;
    }
    Sio32VBlank();
}
