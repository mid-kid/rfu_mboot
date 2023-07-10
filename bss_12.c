#include <Agb.h>

__attribute__((nocommon)) struct RfuBuf {
    u8 *recv;
    u8 *send;
    u8 func[0x60];
} RfuBuf;

__attribute__((nocommon)) u8 RfuDataSendBuf[280];
