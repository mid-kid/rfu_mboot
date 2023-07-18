#include <Agb.h>

#include "Sio32.h"
extern struct Sio32 Sio32;

u32 Sio32IDMain(void)
{
    switch (Sio32.state) {
    case 0:
        Sio32.slave = TRUE;
        *(vu16 *)REG_SIOCNT |= SIO_SCK_IN;

        *(vu16 *)REG_IME = FALSE;
        *(vu16 *)REG_IE |= SIO_INTR_FLAG;
        *(vu16 *)REG_IME = TRUE;

        Sio32.state = 1;
        *(vu8 *)REG_SIOCNT |= SIO_ENABLE;
        break;

    case 1:
        if (Sio32.deviceID == 0) {
            if (Sio32.dataLo == 0x8001) break;
            if (Sio32.slave) break;
            if (Sio32.handshakeStep != 0) break;

            *(vu16 *)REG_IME = FALSE;
            *(vu16 *)REG_IE &= ~SIO_INTR_FLAG;
            *(vu16 *)REG_IME = TRUE;

            *(vu16 *)REG_SIOCNT = 0;
            *(vu16 *)REG_SIOCNT = SIO_32BIT_MODE;
            *(vu16 *)REG_IF = SIO_INTR_FLAG;
            *(vu16 *)REG_SIOCNT |= SIO_IF_ENABLE | SIO_START;

            *(vu16 *)REG_IME = FALSE;
            *(vu16 *)REG_IE |= SIO_INTR_FLAG;
            *(vu16 *)REG_IME = TRUE;
            break;
        }

        Sio32.state = 2;

    default:
        return Sio32.deviceID;
    }

    return 0;
}
