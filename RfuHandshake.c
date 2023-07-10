#include <Agb.h>

extern u32 Sio32InitUpdate(void);
extern void Sio32Init(void);
extern void RfuIntrHandshake(void);
extern u8 u8_03005efc;
extern void (*STWI_intr)(void);

u32 RfuHandshake(void)
{
    u32 ret;
    u32 x;
    u16 ie;

    ie = *(vu16 *)REG_IE;
    u8_03005efc = 1;

    STWI_intr = RfuIntrHandshake;
    Sio32Init();
    for (x = 0; x < 0x3c; x++) {
        VBlankIntrWait();
        ret = Sio32InitUpdate();
        if (ret != 0) break;
    }

    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE = ie;
    *(vu16 *)REG_IME = 1;

    u8_03005efc = 0;
    return ret;
}
