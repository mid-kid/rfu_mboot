#if 1
__asm__("
.section .text
.global RfuWaitDataDone
.type RfuWaitDataDone, function
.thumb_func
RfuWaitDataDone:
.2byte 0x4801,0x7ac0,0x4770,0x0000,0x5ca0,0x0300
.size RfuWaitDataDone, .-RfuWaitDataDone
");
#else
#endif
