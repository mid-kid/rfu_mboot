#if 1
__asm__("
.section .text
@.global FrameCountReset
.type FrameCountReset, function
.thumb_func
FrameCountReset:
.2byte 0x4901,0x2000,0x7008,0x4770,0x5698,0x0300
.size FrameCountReset, .-FrameCountReset
");
#else
#endif
