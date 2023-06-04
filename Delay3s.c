#include <Agb.h>

void Delay3s(void)
{
    u8 i;

    for (i = 0; i < 3 * 60; i++) VBlankIntrWait();
}
