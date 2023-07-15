#include <Agb.h>

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};
extern const struct MenuMsg *MenuMsg;

extern void mf_clearRect(u16 Pos, u8 Height, u8 Width);
extern u16 *mf_drawString(u16 Pos, u16 PlttNo, const char *Srcp);

void MenuMsgSet(u8 Msg, u16 PlttNo)
{
    mf_clearRect(MenuMsg->pos[Msg] & -0x20, 2, 0x20);
    mf_drawString(MenuMsg->pos[Msg], PlttNo, MenuMsg->msg[Msg]);
}
