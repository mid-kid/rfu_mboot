#include <Agb.h>

extern char GameName[14];
extern u16 RfuGameInfoSet(u8 GameSpecial, u16 GameID, char *GameName, char *UserName);
extern char *UserNames[4];
extern u8 MbootPeer;

u16 SearchProc_GameInfoSet(void)
{
    return RfuGameInfoSet(TRUE, 0, GameName, UserNames[MbootPeer]);
}
