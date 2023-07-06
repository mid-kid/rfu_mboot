#ifndef RfuPeer_h
#define RfuPeer_h

struct RfuPeerSub {
    u16 unk_01[1];
    u8 *unk_02[4];
    int unk_03;
    u16 unk_04;
    u8 unk_05;
    u8 unk_06[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12[4];
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 *unk_18;
    u8 unk_19;
    u8 unk_20;
    u16 unk_21;
    u32 unk_22;
};

struct RfuPeer {
    struct RfuPeerSub sub[2];
    void *mbootDest;
    u32 mbootSize;
};

#endif
