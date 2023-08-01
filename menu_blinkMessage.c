#include <Agb.h>
#include "myFunc.h"

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};
extern const struct MenuMsg *MenuMsg;
extern u8 blink_counter;

extern void menu_drawMessage(u8 Msg, u16 PlttNo);

void menu_blinkMessage(u8 Msg, u8 Rate)
{
    if (Msg == 6) {
        mf_clearRect(MenuMsg->pos[6], 2, 0x20);
    } else {
        mf_clearRect(0x200, 2, 0x20);
    }

    if ((blink_counter & Rate) == 0) menu_drawMessage(Msg, 0);

    blink_counter++;
}
