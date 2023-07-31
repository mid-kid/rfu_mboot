#include <Agb.h>

extern u32 AgbRFU_SoftReset_and_checkID(void);

u16 SearchProc_Boot(void)
{
    if (AgbRFU_SoftReset_and_checkID() == 0x8001) {
        return 0;
    } else {
        return 1;
    }
}
