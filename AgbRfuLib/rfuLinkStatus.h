#ifndef Mboot_h
#define Mboot_h

#include "GameInfo.h"

struct rfuLinkStatus {
    u8 mode;
    u8 unk_01;
    u8 peersConn;
    u8 peersSeen;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 gamesCount;
    u8 timer;
    u8 unk_0a[4];
    u8 unk_0e;
    u8 unk_0f;
    u8 unk_10[4];
    struct GameInfo games[4];
    struct GameInfo curGame;
};

#endif
