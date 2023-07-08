#include <Agb.h>

#include "Mboot.h"
#include "MbootTmp.h"
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;

void RfuSetUnk04(u8 val)
{
    struct Mboot *ptr = &Mboot;
    MbootTmp.unk_04 = val;
    ptr->unk_09 = val;
}
