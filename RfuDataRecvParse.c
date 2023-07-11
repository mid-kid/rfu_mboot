#if 1
__asm__("
.text
	.align	2
	.globl	RfuDataRecvParse
	.type	 RfuDataRecvParse,function
	.thumb_func
RfuDataRecvParse:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	add	sp, sp, #-8
	mov	r8, r1
	lsl	r2, r2, #16
	lsr	r3, r2, #16
	ldr	r2, .L20
	ldrb	r0, [r2]
	mvn	r0, r0
	mov	r1, #1
	and	r0, r0, r1
	lsl	r0, r0, #4
	ldr	r1, .L20+4
	add	r6, r0, r1
	ldrb	r0, [r6]
	mov	r9, r2
	cmp	r3, r0
	bcs	.L3	@cond_branch
	add	r0, r3, #0
	b	.L19
.L21:
	.align	2
.L20:
	.word	Mboot
	.word	RfuEncTable
.L3:
	mov	r7, #0
	mov	r4, #0
	ldrb	r5, [r6, #1]
	ldrb	r3, [r6, #7]
	ldr	r1, [sp]
	mov	ip, r1
	cmp	r4, r0
	bcs	.L5	@cond_branch
	add	r2, r0, #0
.L7:
	mov	r1, r8
	ldrb	r0, [r1]
	lsl	r1, r4, #3
	lsl	r0, r0, r1
	orr	r7, r7, r0
	mov	r0, #1
	add	r8, r0
	add	r0, r4, #1
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, r2
	bcc	.L7	@cond_branch
.L5:
	add	r0, r7, #0
	lsr	r0, r0, r5
	and	r0, r0, r3
	ldr	r5, .L22
	add	r3, r5, #0
	mov	r1, ip
	and	r3, r3, r1
	orr	r3, r3, r0
	str	r3, [sp]
	add	r0, r7, #0
	ldrb	r1, [r6, #2]
	lsr	r0, r0, r1
	ldrb	r1, [r6, #8]
	and	r0, r0, r1
	lsl	r0, r0, #8
	ldr	r4, .L22+4
	add	r2, r4, #0
	and	r2, r2, r3
	orr	r2, r2, r0
	str	r2, [sp]
	add	r0, r7, #0
	ldrb	r1, [r6, #3]
	lsr	r0, r0, r1
	ldrb	r1, [r6, #9]
	and	r0, r0, r1
	lsl	r0, r0, #16
	ldr	r3, .L22+8
	and	r3, r3, r2
	orr	r3, r3, r0
	str	r3, [sp]
	add	r0, r7, #0
	ldrb	r1, [r6, #4]
	lsr	r0, r0, r1
	ldrb	r1, [r6, #10]
	and	r0, r0, r1
	lsl	r0, r0, #24
	ldr	r1, .L22+12
	mov	ip, r1
	and	r1, r1, r3
	orr	r1, r1, r0
	mov	ip, r1
	str	r1, [sp]
	add	r0, r7, #0
	ldrb	r1, [r6, #5]
	lsr	r0, r0, r1
	ldrb	r1, [r6, #11]
	and	r0, r0, r1
	ldr	r2, [sp, #4]
	and	r2, r2, r5
	orr	r2, r2, r0
	str	r2, [sp, #4]
	add	r0, r7, #0
	ldrb	r1, [r6, #6]
	lsr	r0, r0, r1
	ldrb	r1, [r6, #12]
	and	r0, r0, r1
	lsl	r0, r0, #8
	and	r4, r4, r2
	orr	r4, r4, r0
	str	r4, [sp, #4]
	ldrh	r0, [r6, #14]
	and	r7, r7, r0
	lsl	r1, r7, #16
	ldr	r0, .L22+16
	and	r0, r0, r4
	orr	r0, r0, r1
	str	r0, [sp, #4]
	lsr	r0, r0, #16
	ldrb	r6, [r6]
	add	r0, r0, r6
	lsl	r0, r0, #16
	lsr	r7, r0, #16
	mov	r0, sp
	ldrb	r0, [r0]
	cmp	r0, #0
	bne	.L9	@cond_branch
	mov	r1, r9
	ldrb	r0, [r1, #2]
	mov	r1, ip
	lsr	r5, r1, #8
	and	r5, r5, r0
	cmp	r5, #0
	beq	.L9	@cond_branch
	mov	r4, #0
	mov	r6, #1
.L13:
	add	r0, r5, #0
	asr	r0, r0, r4
	and	r0, r0, r6
	cmp	r0, #0
	beq	.L12	@cond_branch
	ldr	r0, [sp]
	lsr	r0, r0, #24
	cmp	r0, #0
	bne	.L15	@cond_branch
	lsl	r0, r4, #24
	lsr	r0, r0, #24
	mov	r1, sp
	mov	r2, r8
	bl	RfuDataRecvHandle2
	b	.L12
.L23:
	.align	2
.L22:
	.word	-256
	.word	-65281
	.word	-16711681
	.word	16777215
	.word	65535
.L15:
	ldr	r0, .L24
	ldrb	r0, [r0, #4]
	asr	r0, r0, r4
	and	r0, r0, r6
	cmp	r0, #0
	beq	.L12	@cond_branch
	lsl	r1, r4, #24
	lsr	r1, r1, #24
	add	r0, r1, #0
	mov	r2, sp
	mov	r3, r8
	bl	RfuDataRecvHandle1
.L12:
	add	r0, r4, #1
	lsl	r0, r0, #16
	lsr	r4, r0, #16
	cmp	r4, #3
	bls	.L13	@cond_branch
.L9:
	add	r0, r7, #0
.L19:
	add	sp, sp, #8
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L25:
	.align	2
.L24:
	.word	Mboot
.Lfe1:
	.size	 RfuDataRecvParse,.Lfe1-RfuDataRecvParse
");
#else

#include <Agb.h>

#include "Mboot.h"
extern void RfuDataRecvHandle2(u8 param_1, u8 *param_2, u8 *param_3);
extern void RfuDataRecvHandle1(u8 param_1, u8 param_2, u8 *param_3, u8 *param_4);
extern struct Mboot Mboot;
extern u8 RfuEncTable[2][16];

u16 RfuDataRecvParse(u32 unused, u8 *Srcp, u16 Size)
{
    u16 x;
    u8 *enc;
    u32 bits;
    u32 fields[2];
    u16 ret;
    u8 mode;

    mode = ~Mboot.mode & 1;
    enc = RfuEncTable[mode];
    if (Size < enc[0]) return Size;

    bits = 0;
    for (x = 0; x < enc[0]; x++) {
        bits |= *Srcp++ << x * 8;
    }

    enc = RfuEncTable[mode];
    fields[0] = (bits >> enc[1] & enc[7]) << 0 | (fields[0] & 0xFFFFFF00);
    fields[0] = (bits >> enc[2] & enc[8]) << 8 | (fields[0] & 0xFFFF00FF);
    fields[0] = (bits >> enc[3] & enc[9]) << 16 | (fields[0] & 0xFF00FFFF);
    fields[0] = (bits >> enc[4] & enc[10]) << 24 | (fields[0] & 0x00FFFFFF);
    fields[1] = (bits >> enc[5] & enc[11]) << 0 | (fields[1] & 0xFFFFFF00);
    fields[1] = (bits >> enc[6] & enc[12]) << 8 | (fields[1] & 0xFFFF00FF);
    fields[1] = (bits & *(u16 *)(enc + 14)) << 16 | (fields[1] & 0x0000FFFF);
    ret = fields[0] + enc[0];

    if (*(u8 *)fields == 0) {
        u32 temp = (fields[1] >> 8) & Mboot.peersConn;

        for (x = 0; x < 4; x++) {
            if (!(temp >> x & 1)) continue;
            if ((fields[0] >> 24) == 0) {
                RfuDataRecvHandle2(x, (u8 *)fields, Srcp);
            } else if (Mboot.unk_04 & (1 << x)) {
                RfuDataRecvHandle1(x, x, (u8 *)fields, Srcp);
            }
        }
    }

    return ret;
}
#endif
