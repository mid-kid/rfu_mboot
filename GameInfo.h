#ifndef GameInfo_h
#define GameInfo_h

#include <Agb.h>

struct GameInfo {
    u16 beaconID;
    u8 gameIndex;
    u8 isMultiboot;
    u16 gameID;
    char gameName[14 + 1];
    char userName[8 + 1];
};

#endif
