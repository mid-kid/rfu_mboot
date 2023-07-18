#include <Agb.h>

#include "rfuLinkStatus.h"
extern void rfu_STC_NI_receive_Receiver(u8 param_1, u8 *param_2, u8 *param_3);
extern void rfu_STC_NI_receive_Sender(u8 param_1, u8 param_2, u8 *param_3, u8 *param_4);
extern struct rfuLinkStatus rfuLinkStatus;
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

u16 rfu_STC_analyzeLLSF(u32 unused, u8 *Srcp, u16 Size)
{
    u16 x;
    struct RfuEnc *enc;
    u32 bits;
    struct {u8 _1; u8 _2; u8 _3; u8 _4; u8 _5; u8 _6; u16 _7;} fields;
    u16 ret;
    u8 mode;

    mode = ~rfuLinkStatus.mode & 1;
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
        int temp = rfuLinkStatus.peersConn & (fields._2);
        if(temp) {
            for (x = 0; x < 4; x++) {
                if(!(temp >> x & 1)) continue;
                if ((fields._4) == 0) {
                    rfu_STC_NI_receive_Receiver(x, (u8 *)&fields, Srcp);
                } else if (rfuLinkStatus.unk_04 & (1 << x)) {
                    rfu_STC_NI_receive_Sender(x, x, (u8 *)&fields, Srcp);
                }
            }
        }
    }

    return ret;
}
