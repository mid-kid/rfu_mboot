#include <Agb.h>

#include "Mboot.h"
extern u16 RfuCmd_GameInfoSet(char *GameData, char *UserName);
extern struct Mboot Mboot;

u16 RfuGameInfoSet(u8 IsMultiboot, u16 GameID, char *GameName, char *UserName)
{
    u16 ret;
    u8 x;
    char *GameNameNew;
    char *UserNameCur;
    char *GameNameCur;
    char GameNameTmp[16];

    for (x = 2; x < 16; x++) {
        GameNameTmp[x] = *GameName++;
    }

    GameNameTmp[0] = GameID >> 0;
    GameNameTmp[1] = GameID >> 8;
    if (IsMultiboot) GameNameTmp[1] |= 0x80;

    ret = RfuCmd_GameInfoSet(GameNameTmp, UserName);
    if (ret != 0) return ret;

    Mboot.curGame.isMultiboot = IsMultiboot;
    Mboot.curGame.gameID = GameID;

    GameNameCur = Mboot.curGame.gameName;
    UserNameCur = Mboot.curGame.userName;
    GameNameNew = GameNameTmp + 2;

    for (x = 0; x < 0xe; x++) {
        *GameNameCur++ = *GameNameNew++;

        if (x < 8) {
            if (*UserName != '\0') {
                *UserNameCur = *UserName++;
            } else {
                *UserNameCur = '\0';
            }
            UserNameCur++;
        }
    }

    return ret;
}
