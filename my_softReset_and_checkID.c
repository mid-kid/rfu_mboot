#include <Agb.h>

extern u32 AgbRFU_softReset_and_checkID(void);

u16 my_softReset_and_checkID(void)
{
    if (AgbRFU_softReset_and_checkID() == 0x8001) {
        return 0;
    } else {
        return 1;
    }
}
