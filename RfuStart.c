#include <Agb.h>

extern u16 RfuCmd_Start(void);
extern void Init_Unk292c(void);

u16 RfuStart(void)
{
    u16 ret;

    ret = RfuCmd_Start();
    if (ret == 0) Init_Unk292c();
    return ret;
}
