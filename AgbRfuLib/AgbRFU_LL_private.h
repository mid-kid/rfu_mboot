#ifndef __RFU_LINK_LAYER_PRV_H__
#define __RFU_LINK_LAYER_PRV_H__

typedef struct RFU_staticTag {
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 timer;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 unk_10;
    u8 unk_11;
    volatile u8 unk_12;
    u16 beaconID;
    u8 unk_13;
    u8 unk_14;
}RFU_STATIC;

typedef struct RFU_fixedTag {
    u8 *recv;  // dst
    void (*fastCopy_p)();
    u8 fastCopy_buff[0x60];
    u8 STWI_buf[280];
}RFU_FIXED;

extern RFU_STATIC rfuStatic;
extern RFU_FIXED rfuFixed;

#endif
