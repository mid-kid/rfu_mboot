#include <Agb.h>

#include "Mboot.h"
extern void RfuDataRecvHandle2(u8 param_1, u8 *param_2, u8 *param_3);
extern void RfuDataRecvHandle1(u8 param_1, u8 param_2, u8 *param_3, u8 *param_4);
extern struct Mboot Mboot;
extern struct RfuEnc {
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08;
    u8 unk_09;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
    u16 unk_15;
} RfuEncTable[2];

u16 RfuDataRecvParse(u32 unused, u8 *Srcp, u16 Size)
{
    u16 x;
    struct RfuEnc *enc;
    u32 bits;
    struct {u8 _1; u8 _2; u8 _3; u8 _4; u8 _5; u8 _6; u16 _7;} fields;
    u16 ret;
    u8 mode;

    mode = ~Mboot.mode & 1;
    enc = &RfuEncTable[mode];
    if (Size < enc->unk_01) return Size;

    bits = 0;
    for (x = 0; x < enc->unk_01; x++) {
        bits |= *Srcp++ << x * 8;
    }

    fields._1 = bits >> enc->unk_02 & enc->unk_08;
    fields._2 = bits >> enc->unk_03 & enc->unk_09;
    fields._3 = bits >> enc->unk_04 & enc->unk_10;
    fields._4 = bits >> enc->unk_05 & enc->unk_11;
    fields._5 = bits >> enc->unk_06 & enc->unk_12;
    fields._6 = bits >> enc->unk_07 & enc->unk_13;
    fields._7 = enc->unk_15 & bits;
    ret = fields._7 + enc->unk_01;

    if (fields._1 == 0) {
        int temp = Mboot.peersConn & (fields._2);
        if(temp) {
            for (x = 0; x < 4; x++) {
                if(!(temp >> x & 1)) continue;
                if ((fields._4) == 0) {
                    RfuDataRecvHandle2(x, (u8 *)&fields, Srcp);
                } else if (Mboot.unk_04 & (1 << x)) {
                    RfuDataRecvHandle1(x, x, (u8 *)&fields, Srcp);
                }
            }
        }
    }

    return ret;
}
