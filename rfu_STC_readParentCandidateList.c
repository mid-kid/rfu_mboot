#include <Agb.h>

#include "rfuLinkStatus.h"
extern struct rfuFixed {
    u8 *recv;
    u8 *send;
} rfuFixed;
extern struct rfuLinkStatus rfuLinkStatus;

void rfu_STC_readParentCandidateList(void)
{
    u8 x;
    u8 *data;
    u8 len;
    u8 peer;
    char *tmp;

    data = rfuFixed.recv;
    len = data[1];
    data += 4;

    rfuLinkStatus.gamesCount = 0;
    for (peer = 0; peer < 4 && len != 0; peer++) {

        rfuLinkStatus.games[peer].beaconID = *((u16 *)data);
        data += 2;
        rfuLinkStatus.games[peer].playerNum = *((u8 *)data);
        data += 2;
        rfuLinkStatus.games[peer].gameID = *(u16 *)data & 0x7fff;
        if (*(u16*)data & 0x8000) {
            rfuLinkStatus.games[peer].isMultiboot = 1;
        } else {
            rfuLinkStatus.games[peer].isMultiboot = 0;
        }
        data += 2;

        tmp = rfuLinkStatus.games[peer].gameName;
        for (x = 0; x < 14; x++) *tmp++ = *data++;

        tmp = rfuLinkStatus.games[peer].userName;
        for (x = 0; x < 8; x++) *tmp++ = *data++;

        len -= 7;
        rfuLinkStatus.gamesCount++;
    }
}
