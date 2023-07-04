#include <Agb.h>

void RfuMemcpy(u8 **Src, u8 **Dst, int Size)
{
    u8 *src = *Src;
    u8 *dst = *Dst;

    while (Size--) *dst++ = *src++;

    *Src = src;
    *Dst = dst;
}
