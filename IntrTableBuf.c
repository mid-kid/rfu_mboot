#include <Agb.h>

extern void Sio32Intr(void);
extern void VBlankIntr(void);

void (*const IntrTableBuf[])(void) = {
    Sio32Intr,
    VBlankIntr
};
