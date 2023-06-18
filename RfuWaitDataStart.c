#include <Agb.h>

#include "Rfu.h"
extern struct Rfu Rfu;
extern u16 RfuCmd_WaitData(void);

extern struct {
    u8 _;
    u8 field1_0x1;
} struct_unk_6180;

u16 RfuWaitDataStart(u8 param_1)
{
    u16 ret = 0;

    if (param_1 == 0) {
        if (Rfu.modeMaster == TRUE) {
            ret = RfuCmd_WaitData();
            if (ret == 0) struct_unk_6180.field1_0x1 = 0;
        }
    } else if (Rfu.modeMaster == FALSE) {
        struct_unk_6180.field1_0x1 = param_1;
    }

    return ret;
}
