#ifndef NONMATCHING
__asm__("
.text
	.align	2
	.globl	RfuMbootDLStart
	.type	 RfuMbootDLStart,function
	.thumb_func
RfuMbootDLStart:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	lsl	r0, r0, #24
	lsr	r6, r0, #24
	lsl	r1, r1, #24
	lsr	r3, r1, #24
	lsl	r2, r2, #16
	lsr	r2, r2, #16
	mov	r8, r2
	ldr	r0, .L37
	ldrb	r1, [r0]
	mov	r9, r0
	cmp	r1, #255
	bne	.L3	@cond_branch
	ldr	r0, .L37+4
	b	.L35
.L38:
	.align	2
.L37:
	.word	Mboot
	.word	1282
.L3:
	mov	r0, #15
	and	r0, r0, r3
	cmp	r0, #0
	bne	.L5	@cond_branch
	ldr	r0, .L39
	b	.L35
.L40:
	.align	2
.L39:
	.word	1537
.L5:
	mov	r1, r9
	ldrb	r0, [r1, #2]
	and	r0, r0, r3
	cmp	r0, r3
	beq	.L7	@cond_branch
	ldr	r0, .L41
	b	.L35
.L42:
	.align	2
.L41:
	.word	1538
.L7:
	mov	r2, r9
	ldrb	r0, [r2, #4]
	and	r0, r0, r3
	cmp	r0, #0
	beq	.L9	@cond_branch
	ldr	r0, .L43
	b	.L35
.L44:
	.align	2
.L43:
	.word	1539
.L9:
	mov	r1, #0
	mov	r0, #1
	and	r0, r0, r3
	mov	r5, r9
	add	r5, r5, #16
	ldr	r2, .L45
	cmp	r0, #0
	bne	.L12	@cond_branch
	mov	r4, #1
.L13:
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bhi	.L12	@cond_branch
	add	r0, r3, #0
	asr	r0, r0, r1
	and	r0, r0, r4
	cmp	r0, #0
	beq	.L13	@cond_branch
.L12:
	add	r7, r1, r5
	add	r0, r5, #0
	sub	r0, r0, #16
	ldrb	r0, [r0]
	lsl	r0, r0, #4
	add	r0, r0, r2
	ldrb	r5, [r0]
	ldrb	r0, [r7]
	cmp	r8, r0
	bhi	.L18	@cond_branch
	cmp	r8, r5
	bhi	.L17	@cond_branch
.L18:
	mov	r0, #224
	lsl	r0, r0, #3
	b	.L35
.L46:
	.align	2
.L45:
	.word	RfuEncTable
.L17:
	mov	r0, #32
	and	r0, r0, r6
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, #0
	bne	.L21	@cond_branch
	cmp	r6, #64
	bne	.L20	@cond_branch
.L21:
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	ldr	r1, .L47
	add	r0, r0, r1
	mov	ip, r0
	mov	r1, #0
	strh	r1, [r0, #24]
	mov	r1, ip
	add	r1, r1, #45
	mov	r2, ip
	str	r1, [r2, #4]
	mov	r0, #7
	str	r0, [r2, #20]
	mov	r0, ip
	add	r0, r0, #44
	strb	r3, [r0]
	strb	r3, [r2, #26]
	mov	r2, r8
	sub	r0, r2, r5
	mov	r2, ip
	strh	r0, [r2, #46]
	cmp	r4, #0
	beq	.L22	@cond_branch
	mov	r0, #0
	b	.L36
.L48:
	.align	2
.L47:
	.word	RfuPeers
.L22:
	mov	r0, #1
.L36:
	strb	r0, [r1]
	ldr	r0, [sp, #32]
	mov	r1, ip
	str	r0, [r1, #48]
	mov	r2, sl
	str	r2, [r1, #40]
	mov	r1, #0
	mov	r0, ip
	strb	r1, [r0, #31]
	add	r0, r0, #32
	strb	r1, [r0]
	mov	r4, ip
	add	r4, r4, #27
	mov	r6, #0
	mov	r2, ip
	add	r2, r2, #33
	mov	r5, #1
.L27:
	add	r0, r4, r1
	strb	r6, [r0]
	add	r0, r2, r1
	strb	r5, [r0]
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bls	.L27	@cond_branch
	mov	r1, #0
	mov	r6, #1
	ldr	r5, .L49
	mov	r4, #0
.L32:
	add	r0, r3, #0
	asr	r0, r0, r1
	and	r0, r0, r6
	cmp	r0, #0
	beq	.L31	@cond_branch
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	add	r0, r0, r5
	strh	r4, [r0, #2]
.L31:
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bls	.L32	@cond_branch
	mov	r2, r9
	ldrb	r1, [r2, #4]
	add	r0, r3, #0
	orr	r0, r0, r1
	strb	r0, [r2, #4]
	ldrb	r0, [r7]
	mov	r1, r8
	sub	r0, r0, r1
	strb	r0, [r7]
	ldr	r0, .L49+4
	mov	r2, ip
	strh	r0, [r2]
.L20:
	mov	r0, #0
.L35:
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L50:
	.align	2
.L49:
	.word	RfuPeers
	.word	32801
.Lfe1:
	.size	 RfuMbootDLStart,.Lfe1-RfuMbootDLStart
");
#else

#include <Agb.h>

#include "Mboot.h"
#include "RfuPeer.h"
extern struct Mboot Mboot;
extern u8 RfuEncTable[2][16];
extern struct RfuPeer RfuPeers[4];

u16 RfuMbootDLStart(u8 param_1, u8 param_2, u16 param_3, u16 *GameID, u32 param_5)
{
    u8 peer;
    u8 max;
    u8 *min;
    struct RfuPeerSub *sub;

    if (Mboot.mode == 0xff) {
        return 0x502;
    } else if ((param_2 & 0xf) == 0) {
        return 0x601;
    } else if ((Mboot.peersConn & param_2) != param_2) {
        return 0x602;
    } else if ((Mboot.unk_04 & param_2) != 0) {
        return 0x603;
    } else {
        for (peer = 0; peer < 4; peer++) {
            if (param_2 >> peer & 1) break;
        }
        min = &Mboot.unk_10[peer];
        max = RfuEncTable[Mboot.mode][0];
        if (param_3 > *min || param_3 <= max) {
            return 0x700;
        } else {
            if ((param_1 & 0x20) != 0 || param_1 == 0x40) {
                sub = &RfuPeers[peer].sub[0];
                sub->unk_04 = 0;
                sub->unk_02[0] = &sub->unk_20;
                sub->unk_03 = 7;
                sub->unk_19 = param_2;
                sub->unk_05 = param_2;
                sub->unk_21 = param_3 - max;
                if (param_1 & 0x20) {
                    sub->unk_20 = 0;
                } else {
                    sub->unk_20 = 1;
                }
                sub->unk_22 = param_5;
                sub->unk_18 = (u8 *)GameID;
                sub->unk_10 = 0;
                sub->unk_11 = 0;

                for (peer = 0; peer < 4; peer++) {
                    sub->unk_06[peer] = 0;
                    sub->unk_12[peer] = 1;
                }

                for (peer = 0; peer < 4; peer++) {
                    if (param_2 >> peer & 1) {
                        RfuPeers[peer].sub[0].unk_01[1] = 0;
                    }
                }
                Mboot.unk_04 |= param_2;
                *min -= param_3;
                sub->unk_01[0] = 0x8021;
            }
            return 0;
        }
    }
}

#endif
