#include <Agb.h>

u16 Bg0Bak[32*20];
void (*nowProcess)();

struct Keys {
    u16 trg;
    u16 cont;
} key;
