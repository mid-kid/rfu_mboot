#include <Agb.h>

extern u16 RfuCmd_WaitData(void);

extern struct {
    u8 _;
    u8 field1_0x1;
} struct_unk_6180;

u16 RfuWaitDataStartForce(void)
{
    u16 ret = 0;

    if (struct_unk_6180.field1_0x1 == 0) {
        ret = RfuCmd_WaitData();
    } else {
        struct_unk_6180.field1_0x1 = 0;
    }

    return ret;
}
