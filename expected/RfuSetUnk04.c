#if 1
__asm__("
.section .text
.global RfuSetUnk04
.type RfuSetUnk04, function
.thumb_func
RfuSetUnk04:
.2byte 0x0600,0x0e00,0x4a02,0x4903,0x70c8,0x7250,0x4770,0x0000,0x60c0,0x0300,0x6180,0x0300
.size RfuSetUnk04, .-RfuSetUnk04
");
#else

void RfuSetUnk04(u8 param_1)
{
    MbootTmp.unk_04 = param_1;
    Mboot.unk_09 = param_1;
    return;
}
#endif
