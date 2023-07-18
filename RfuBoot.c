#include <Agb.h>

extern void RfuResetAdapter(void);
extern u32 AgbRFU_checkID(void);

u32 RfuBoot(void)
{
    RfuResetAdapter();
    return AgbRFU_checkID();
}
