static const char HandshakeString[] = {'N', 'I', 'N', 'T', 'E', 'N', 'D', 'O'};

#if 1
__asm__("
.section .text
.global RfuIntrHandshake
.type RfuIntrHandshake, function
.thumb_func
RfuIntrHandshake:
.2byte 0xb530,0x4811,0x6803,0x4811,0x7801,0x1c04,0x2901,0xd004,0x480f,0x8801,0x2280,0x4311,0x8001,0x7822,0x0111,0x1c18,0x4088,0x0c01,0x2001,0x1a80,0x0100,0x4083,0x1c18,0x0c03,0x8960,0x2800,0xd11b,0x88e2,0x4291,0xd117,0x8865,0x2d03,0xd908,0x8163,0xe013,0x0000,0x0120,0x0400,0x5738,0x0300,0x0128,0x0400,0x88a0,0x43c0,0x0400,0x0c00,0x4281,0xd106,0x43d0,0x0400,0x0c00,0x4283,0xd101,0x1c68,0x8060,0x8860,0x2803,0xd807,0x0040,0x4902,0x1840,0x8800,0xe003,0x0000,0x5630,0x0300,0x480e,0x80a0,0x43d8,0x80e0,0x4b0d,0x88a2,0x7821,0x2001,0x1a40,0x0100,0x4082,0x88e0,0x0109,0x4088,0x1812,0x601a,0x7820,0x2801,0xd107,0x8960,0x2800,0xd104,0x4805,0x8801,0x2280,0x4311,0x8001,0xbc30,0xbc01,0x4700,0x8001,0x0000,0x0120,0x0400,0x0128,0x0400
.size RfuIntrHandshake, .-RfuIntrHandshake
");
#else

#include <Agb.h>

#include "Sio32.h"
extern struct Sio32 Sio32;

void RfuIntrHandshake(void)
{
    u32 data;
    u32 DataHi;
    u32 DataLo;

    data = *(vu32 *)REG_SIODATA32;
    if (Sio32.slave != TRUE) {
        *(vu16 *)REG_SIOCNT |= 0x80;
    }

    DataHi = data << ((0 + Sio32.slave) * 16) >> 16;
    DataLo = data << ((1 - Sio32.slave) * 16) >> 16;

    if (Sio32.deviceID == 0) {
        if (DataHi == Sio32.dataHi) {
            if (Sio32.handshakeStep < 4) {
                if ((u16)~Sio32.dataLo == DataHi && (u16)~Sio32.dataHi == DataLo) {
                    Sio32.handshakeStep++;
                }
            } else {
                Sio32.deviceID = DataLo;
            }
        } else {
            Sio32.handshakeStep = 0;
        }
    }
    if (Sio32.handshakeStep < 4) {
        Sio32.dataLo = *(u16 *)(HandshakeString + Sio32.handshakeStep * 2);
    } else {
        // Send device ID
        Sio32.dataLo = 0x8001;
    }
    Sio32.dataHi = ~DataLo;

    *(vu32 *)REG_SIODATA32 =
        (Sio32.dataLo << ((1 - Sio32.slave) * 0x10)) +
        (Sio32.dataHi << ((0 + Sio32.slave) * 0x10));

    if (Sio32.slave == TRUE && Sio32.deviceID == 0) {
        *(vu16 *)REG_SIOCNT |= 0x80;
    }
}
#endif
