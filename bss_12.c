#include <Agb.h>

struct rfuFixed {
    u8 *recv;
    u8 *send;
    u8 func[0x60];
    u8 buf[280];
} rfuFixed;
