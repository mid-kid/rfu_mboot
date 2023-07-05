#include <Agb.h>

extern u16 RfuCmd_Start(void);
extern void MbootInit(void);

u16 RfuStart(void)
{
    u16 ret;

    ret = RfuCmd_Start();
    if (ret == 0) MbootInit();
    return ret;
}
