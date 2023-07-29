#ifndef RfuPeer_h
#define RfuPeer_h

struct RfuPeerSub {
    u16 unk_01[2];  // state, failCounter
    u8 *unk_02[4];  // nowp
    u32 unk_03;  // remainSize
    u16 unk_04;  // errorCode
    u8 unk_05;  // bmSlot
    u8 unk_06[4];  // recv_ack_flag
    u8 unk_10;  // ack
    u8 unk_11;  // phase
    u8 unk_12[4];  // n
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
    u8 *unk_18;  // src
    u8 unk_19;  // bmSlot_org
    u8 unk_20;  // dataType
    u16 unk_21;  // payloadSize
    u32 unk_22;  // dataSize
};

struct RfuPeer {
    struct RfuPeerSub sub[2];
    u8 *mbootDest;
    u32 mbootSize;
};

#endif
