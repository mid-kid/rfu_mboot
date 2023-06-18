#if 1
__asm__("
.section .text
.global RfuWaitData
.type RfuWaitData, function
.thumb_func
RfuWaitData:
.2byte 0xb500,0x2001,0xf001,0xfff8,0xf002,0xf82e,0x0600,0x2800,0xd0fa,0xbc01,0x4700,0x0000
.size RfuWaitData, .-RfuWaitData
");
#else
#endif
