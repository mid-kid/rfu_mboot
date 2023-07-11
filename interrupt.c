#include <Agb.h>

extern void Sio32Intr(void);
extern void VBlankIntr(void);

void (*const IntrTable[])(void) = {
    Sio32Intr,
    VBlankIntr
};
