#if 1
__asm__("
.section .text
@.global SoundPlaySfx
.type SoundPlaySfx, function
.thumb_func
SoundPlaySfx:
.2byte 0x0600,0x0e00,0x1c02,0x4b08,0x7a58,0x4290,0xd902,0x7a18,0x28ff,0xd108,0x4905,0x0090,0x1840,0x6800,0x6058,0x6018,0x2000,0x7218,0x725a,0x4770,0x5780,0x0300,0x43a4,0x0300
.size SoundPlaySfx, .-SoundPlaySfx
");
#else
#endif