#include <Agb.h>

extern void RfuResetAdapter(void);
extern u32 RfuHandshake(void);

u32 RfuBoot(void)
{
    RfuResetAdapter();
    return RfuHandshake();
}
