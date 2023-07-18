#include <Agb.h>

void rfu_STC_fastCopy(u8 **Src, u8 **Dst, int Size)
{
    u8 *src = *Src;
    u8 *dst = *Dst;

    while (Size--) *dst++ = *src++;

    *Src = src;
    *Dst = dst;
}
