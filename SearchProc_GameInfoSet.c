#include <Agb.h>

extern char GameName[14];
extern u16 rfu_REQ_configGameData(u8 GameSpecial, u16 GameID, char *GameName, char *UserName);
extern char *UserNames[4];
extern u8 MbootPeer;

u16 SearchProc_GameInfoSet(void)
{
    return rfu_REQ_configGameData(TRUE, 0, GameName, UserNames[MbootPeer]);
}
