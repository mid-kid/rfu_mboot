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
