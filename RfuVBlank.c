#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern void Sio32VBlank(void);

void RfuVBlank(void)
{
    if (Mboot.mode != (u8)-1) {
        if (MbootTmp.unk_04) MbootTmp.unk_04--;
    }
    Sio32VBlank();
}
