#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;
extern u32 MbootReset(void);

void MbootInit(void)
{
    u8 save;

    CpuClear(0, &MbootTmp, 16, 16);
    save = Mboot.unk_09;
    CpuClear(0, &Mboot, 0x5a*2, 16);
    MbootTmp.unk_04 = save;
    Mboot.unk_09 = save;
    Mboot.mode = 0xff;
    MbootReset();
}
