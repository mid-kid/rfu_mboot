#if 1
__asm__("
.section .text
@.global RfuWaitDataStart
.type RfuWaitDataStart, function
.thumb_func
RfuWaitDataStart:
.2byte 0xb500,0x0600,0x0e00,0x1c01,0x2200,0x2900,0xd110,0x4806,0x7ac0,0x2801,0xd112,0xf7ff,0xfaa3,0x0400,0x0c02,0x2a00,0xd10c,0x4802,0x7042,0xe009,0x5ca0,0x0300,0x6180,0x0300,0x4804,0x7ac0,0x2800,0xd101,0x4803,0x7041,0x1c10,0xbc02,0x4708,0x0000,0x5ca0,0x0300,0x6180,0x0300
.size RfuWaitDataStart, .-RfuWaitDataStart
");
#else
#endif
