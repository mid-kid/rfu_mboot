#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;

u8 RfuWaitDataDone(void)
{
    return STWI_status.modeMaster;
}
