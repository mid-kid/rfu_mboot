#include <Agb.h>

#include "Mboot.h"
extern struct RfuBuf {
    u8 *recv;
    u8 *send;
} RfuBuf;
extern struct Mboot Mboot;

void RfuCmd_GameInfoGet_Parse(void)
{
    u8 x;
    u8 *data;
    u8 len;
    u8 peer;
    char *tmp;

    data = RfuBuf.recv;
    len = data[1];
    data += 4;

    Mboot.gamesCount = 0;
    for (peer = 0; peer < 4 && len != 0; peer++) {

        Mboot.games[peer].beaconID = *((u16 *)data);
        data += 2;
        Mboot.games[peer].playerNum = *((u8 *)data);
        data += 2;
        Mboot.games[peer].gameID = *(u16 *)data & 0x7fff;
        if (*(u16*)data & 0x8000) {
            Mboot.games[peer].isMultiboot = 1;
        } else {
            Mboot.games[peer].isMultiboot = 0;
        }
        data += 2;

        tmp = Mboot.games[peer].gameName;
        for (x = 0; x < 14; x++) *tmp++ = *data++;

        tmp = Mboot.games[peer].userName;
        for (x = 0; x < 8; x++) *tmp++ = *data++;

        len -= 7;
        Mboot.gamesCount++;
    }
}
