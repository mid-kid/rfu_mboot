#include <Agb.h>

extern u32 Sio32IDMain(void);
extern void Sio32IDInit(void);
extern void Sio32IDIntr(void);
extern u8 u8_03005efc;
extern void (*STWI_intr)(void);

u32 AgbRFU_checkID(void)
{
    u32 ret;
    u32 x;
    u16 ie;

    ie = *(vu16 *)REG_IE;
    u8_03005efc = 1;

    STWI_intr = Sio32IDIntr;
    Sio32IDInit();
    for (x = 0; x < 0x3c; x++) {
        VBlankIntrWait();
        ret = Sio32IDMain();
        if (ret != 0) break;
    }

    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE = ie;
    *(vu16 *)REG_IME = 1;

    u8_03005efc = 0;
    return ret;
}
