#include <Agb.h>

void RfuResetAdapter(void)
{
    u16 x;

    *(vu16 *)REG_RCNT = 0x8000;

    // Pulse the SD pin
    *(vu16 *)REG_RCNT = 0x80a0;
    for (x = 0; x < 1000; x++) *(vu16 *)REG_RCNT = 0x80a2;
    *(vu16 *)REG_RCNT = 0x80a0;

    *(vu16 *)REG_SIOCNT = 0x5003;
}
