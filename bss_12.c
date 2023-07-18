#include <Agb.h>

__attribute__((nocommon)) struct rfuFixed {
    u8 *recv;
    u8 *send;
    u8 func[0x60];
} rfuFixed;

__attribute__((nocommon)) u8 RfuDataSendBuf[280];
