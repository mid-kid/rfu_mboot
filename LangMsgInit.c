#include <Agb.h>

struct LangMsg {
    const char *const *msg;
    const u16 *pos;
};

extern const struct LangMsg LangMsgInitial[];
extern struct LangMsg *LangMsg;
extern u8 Lang;

void LangMsgInit(void)
{
    LangMsg = LangMsgInitial + Lang;
}
