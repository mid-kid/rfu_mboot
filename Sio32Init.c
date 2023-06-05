#include <Agb.h>

extern char Sio32[12];

void Sio32Init(void)
{
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE &= ~(SIO_INTR_FLAG | TIMER3_INTR_FLAG);
    *(vu16 *)REG_IME = 1;

    *(vu16 *)REG_RCNT = 0;
    *(vu16 *)REG_SIOCNT = SIO_32BIT_MODE;
    *(vu16 *)REG_SIOCNT |= SIO_IF_ENABLE | SIO_ENABLE;
    CpuClear(0, Sio32, sizeof(Sio32), 32);
    *(vu16 *)REG_IF = SIO_INTR_FLAG | TIMER3_INTR_FLAG;
}
