#ifndef NONMATCHING
__asm__("
.section .text
.thumb
.global RfuConnectCheck
.type RfuConnectCheck, function
.thumb_func
RfuConnectCheck:
push	{r4, r5, r6, r7, lr}
mov	r7, sl
mov	r6, r9
mov	r5, r8
push	{r5, r6, r7}
sub	sp, #36
mov	r4, r0
mov	r8, r1
mov	r7, #255
strb	r7, [r4, #0]
strb	r7, [r1, #0]
bl	RfuCmd_ConnectCheck
lsl	r0, r0, #16
lsr	r0, r0, #16
mov	r9, r0
cmp	r0, #0
beq	.LRfuConnectCheck_0x26
b	.LRfuConnectCheck_0x128
.LRfuConnectCheck_0x26:
add	r6, sp, #32
mov	r0, r4
mov	r1, r8
mov	r2, r6
bl	RfuCmd_ConnectCheck_Parse
ldrb	r0, [r4, #0]
cmp	r0, #0
bne	.LRfuConnectCheck_0x128
mov	r0, #128
lsl	r0, r0, #17
mov	r1, r8
ldrb	r1, [r1, #0]
lsl	r0, r1
lsr	r4, r0, #24
ldr	r1, [pc, #92]
ldrb	r5, [r1, #2]
mov	r0, r4
and	r0, r5
mov	r3, r1
cmp	r0, #0
bne	.LRfuConnectCheck_0x128
ldr	r1, [pc, #84]
ldrh	r0, [r1, #0]
mov	sl, r0
mov	r2, #0
mov	r0, r9
strh	r0, [r1, #0]
mov	r0, r4
orr	r0, r5
strb	r0, [r3, #2]
ldrb	r0, [r3, #3]
bic	r0, r4
strb	r0, [r3, #3]
ldrh	r1, [r6, #0]
mov	r0, r3
add	r0, #148
strh	r1, [r0, #0]
ldrb	r0, [r3, #1]
add	r0, #1
strb	r0, [r3, #1]
mov	r1, r3
add	r1, #10
mov	r4, r8
ldrb	r4, [r4, #0]
add	r1, r1, r4
ldrb	r0, [r1, #0]
orr	r0, r7
strb	r0, [r1, #0]
strb	r2, [r3, #0]
mov	r6, #0
ldr	r0, [pc, #28]
ldrh	r1, [r3, #20]
mov	r2, r0
ldrh	r0, [r2, #12]
cmp	r1, r0
bne	.LRfuConnectCheck_0xe0
ldrb	r0, [r3, #8]
cmp	r0, #0
bne	.LRfuConnectCheck_0xb0
mov	r7, r3
add	r7, #20
b	.LRfuConnectCheck_0x102

.4byte Mboot
.4byte 0x04000208
.4byte MbootTmp

.LRfuConnectCheck_0xb0:
mov	r7, sp
lsl	r0, r6, #5
ldr	r4, [pc, #32]
add	r0, r0, r4
mov	r1, sp
mov	r2, #16
bl	CpuSet
mov	r0, sp
add	r0, #34
mov	r5, #0
strh	r5, [r0, #0]
ldr	r2, [pc, #16]
mov	r1, r4
bl	CpuSet
sub	r4, #20
strb	r5, [r4, #8]
b	.LRfuConnectCheck_0x102

.align 2, 0
.4byte Mboot+0x14
.4byte 0x01000040

.LRfuConnectCheck_0xe0:
add	r0, r6, #1
lsl	r0, r0, #24
lsr	r6, r0, #24
cmp	r6, #3
bhi	.LRfuConnectCheck_0x122
lsl	r1, r6, #5
add	r0, r1, r3
ldrh	r0, [r0, #20]
ldrh	r4, [r2, #12]
cmp	r0, r4
bne	.LRfuConnectCheck_0xe0
ldrb	r0, [r3, #8]
cmp	r0, #0
bne	.LRfuConnectCheck_0xb0
mov	r0, r3
add	r0, #20
add	r7, r1, r0
.LRfuConnectCheck_0x102:
cmp	r6, #3
bhi	.LRfuConnectCheck_0x122
mov	r0, r8
ldrb	r1, [r0, #0]
lsl	r1, r1, #5
ldr	r4, [pc, #44]
add	r1, r1, r4
mov	r0, r7
mov	r2, #16
bl	CpuSet
mov	r2, r8
ldrb	r1, [r2, #0]
lsl	r0, r1, #5
add	r4, r4, r0
.LRfuConnectCheck_0x120:
strb	r1, [r4, #2]
.LRfuConnectCheck_0x122:
ldr	r0, [pc, #28]
mov	r4, sl
strh	r4, [r0, #0]
.LRfuConnectCheck_0x128:
mov	r0, r9
add	sp, #36
pop	{r3, r4, r5}
mov	r8, r3
mov	r9, r4
mov	sl, r5
pop	{r4, r5, r6, r7}
pop	{r1}
bx	r1

.align 2, 0
.4byte Mboot+0x14
.4byte 0x04000208
.size RfuConnectCheck, .-RfuConnectCheck
");
#else

#include <Agb.h>

#include "GameInfo.h"
#include "Mboot.h"
#include "MbootTmp.h"
extern u16 RfuCmd_ConnectCheck(void);
extern void RfuCmd_ConnectCheck_Parse(u8 *Busy, u8 *PlayerNum, u16 *ID);
extern struct Mboot Mboot;
extern struct MbootTmp MbootTmp;

u16 RfuConnectCheck(u8 *Busy, u8 *PlayerNum)
{
    u16 ime;
    u16 ret;
    u8 x;
    struct GameInfo *GameSrc;
    struct GameInfo GameTmp;
    u16 ID;
    u8 bit;

    GameSrc = &GameTmp;
    *Busy = -1;
    *PlayerNum = -1;

    ret = RfuCmd_ConnectCheck();
    if (ret != 0) return ret;

    RfuCmd_ConnectCheck_Parse(Busy, PlayerNum, &ID);
    if (*Busy != 0) return 0;

    bit = 1 << *PlayerNum;
    if (Mboot.peersConn & bit) return 0;

    ime = *(vu16 *)REG_IME;
    *(vu16 *)REG_IME = 0;

    Mboot.peersConn |= bit;
    Mboot.peersSeen &= ~bit;

    Mboot.curGame.beaconID = ID;
    Mboot.unk_01++;
    Mboot.unk_0a[*PlayerNum] = -1;
    Mboot.mode = 0;

    for (x = 0; x < 4; x++) {
        if (Mboot.games[x].beaconID == MbootTmp.beaconID) {
            if (Mboot.gamesCount == 0) {
                GameSrc = Mboot.games;
            } else {
                GameSrc = &GameTmp;
                CpuCopy(Mboot.games + x, &GameTmp, 0x10*2, 16);
                CpuArrayClear(0, Mboot.games, 16);
                Mboot.gamesCount = 0;
            }
            break;
        }
    }

    if (x < 4) {
        CpuCopy(GameSrc, Mboot.games + *PlayerNum, 0x10*2, 16);
        Mboot.games[*PlayerNum].playerNum = *PlayerNum;
    }

    *(vu16 *)REG_IME = ime;
    return 0;
}
#endif
