#if 0
__asm__("
.section .text
@.global MenuMsgBlink
.type MenuMsgBlink, function
.thumb_func
MenuMsgBlink:
.2byte 0xb570,0x0600,0x0e05,0x0609,0x0e0e,0x2d06,0xd10a,0x4804,0x6800,0x6840,0x8980,0x2102,0x2220,0xf000,0xf921,0xe007,0x575c,0x0300,0x2080,0x0080,0x2102,0x2220,0xf000,0xf918,0x4c07,0x7820,0x4030,0x2800,0xd103,0x1c28,0x2100,0xf7ff,0xffad,0x7820,0x3001,0x7020,0xbc70,0xbc01,0x4700,0x0000,0x5698,0x0300
.size MenuMsgBlink, .-MenuMsgBlink
");
#else

#include <Agb.h>

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};
extern const struct MenuMsg *MenuMsg;
extern u8 FrameCount;

extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern void MenuMsgSet(u8 Msg, u16 PlttNo);

void MenuMsgBlink(u8 Msg, u8 Rate)
{
    if (Msg == 6) {
        BgScClear(MenuMsg->pos[6], 2, 0x20);
    } else {
        BgScClear(0x200, 2, 0x20);
    }

    if ((FrameCount & Rate) == 0) MenuMsgSet(Msg, 0);

    FrameCount++;
}

#endif
