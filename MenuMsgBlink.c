#include <Agb.h>
#include "myFunc.h"

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};
extern const struct MenuMsg *MenuMsg;
extern u8 FrameCount;

extern void MenuMsgSet(u8 Msg, u16 PlttNo);

void MenuMsgBlink(u8 Msg, u8 Rate)
{
    if (Msg == 6) {
        mf_clearRect(MenuMsg->pos[6], 2, 0x20);
    } else {
        mf_clearRect(0x200, 2, 0x20);
    }

    if ((FrameCount & Rate) == 0) MenuMsgSet(Msg, 0);

    FrameCount++;
}
