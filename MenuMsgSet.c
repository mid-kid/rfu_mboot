#include <Agb.h>

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};

extern const struct MenuMsg *MenuMsg;

extern void BgScClear(u16 Pos, u8 Height, u8 Width);
extern u16 *BgScSet(u16 Pos, u16 PlttNo, char *Srcp);

void MenuMsgSet(u8 Msg, u16 PlttNo)
{
    BgScClear(MenuMsg->pos[Msg] & -0x20, 2, 0x20);
    BgScSet(MenuMsg->pos[Msg], PlttNo, MenuMsg->msg[Msg]);
}
