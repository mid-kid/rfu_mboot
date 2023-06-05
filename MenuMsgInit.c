#include <Agb.h>

struct MenuMsg {
    const char *const *msg;
    const u16 *pos;
};

extern const struct MenuMsg MenuMsgInitial[];
extern const struct MenuMsg *MenuMsg;
extern u8 Lang;

void MenuMsgInit(void)
{
    MenuMsg = MenuMsgInitial + Lang;
}
