#include <Agb.h>
#include "myFunc.h"

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};
extern const struct MenuMsg *MenuMsg;

void menu_drawMessage(u8 Msg, u16 PlttNo)
{
    mf_clearRect(MenuMsg->pos[Msg] & -0x20, 2, 0x20);
    mf_drawString(MenuMsg->pos[Msg], PlttNo, MenuMsg->msg[Msg]);
}
