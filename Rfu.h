#ifndef Rfu_h
#define Rfu_h

struct Rfu {
    u32 state;
    u32 cmdHeader;
    u8 cmdSize;
    u8 field3_0x9;
    u8 error;
    u8 modeMaster;
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
};

#endif
