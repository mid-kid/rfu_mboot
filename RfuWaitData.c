#include <Agb.h>

extern u8 RfuWaitDataStart(u8 param_1);
extern u8 RfuWaitDataDone(void);

void RfuWaitData(void)
{
    RfuWaitDataStart(1);
    while (!RfuWaitDataDone());
}
