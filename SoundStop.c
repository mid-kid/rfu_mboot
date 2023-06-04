#if 1
__asm__("
.section .text
@.global SoundStop
.type SoundStop, function
.thumb_func
SoundStop:
.2byte 0x4802,0x2100,0x7241,0x21ff,0x7201,0x4770,0x5780,0x0300
.size SoundStop, .-SoundStop
");
#else
#endif
