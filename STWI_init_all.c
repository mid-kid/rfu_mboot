#include <Agb.h>

#include "STWI_status.h"
extern struct STWI_status STWI_status;
void STWI_init(void);

u32 STWI_init_all(void)
{
    STWI_init();
    STWI_status.unk_09 = 8;
    STWI_status.unk_12 = 1;
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE |= 0x81;
    *(vu16 *)REG_IME = 1;
    return 0;
}
