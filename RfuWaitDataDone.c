#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;

u8 RfuWaitDataDone(void)
{
    return Rfu.modeMaster;
}
