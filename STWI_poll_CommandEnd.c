#ifndef NONMATCHING
__asm__("
.text
	.align	2
	.globl	STWI_poll_CommandEnd
	.type	 STWI_poll_CommandEnd,function
	.thumb_func
STWI_poll_CommandEnd:
	push	{r4, r5, lr}
	ldr	r0, .LA21
	ldrb	r0, [r0, #11]
	cmp	r0, #0
	bne	.LA4	@cond_branch
	mov	r0, #1
	b	.LA19
.LA22:
	.align	2
.LA21:
	.word	STWI_status
.LA4:
	ldr	r1, .LA23
	ldr	r0, .LA23+4
	ldr	r0, [r0]
	str	r0, [r1]
	ldr	r1, .LA23+8
	str	r0, [r1, #4]
	mov	r0, #1
	strb	r0, [r1, #9]
	ldr	r2, .LA23+12
	ldr	r3, .LA23+16
	add	r0, r3, #0
	strh	r0, [r2]
	add	r2, r1, #0
	add	r2, r2, #12
.LA5:
	ldrb	r0, [r2]
	cmp	r0, #0
	beq	.LA5	@cond_branch
	ldrb	r2, [r1, #13]
	sub	r0, r2, #1
	lsl	r0, r0, #24
	lsr	r0, r0, #24
	cmp	r0, #1
	bhi	.LA7	@cond_branch
	mov	r4, #0
	ldrb	r0, [r1, #14]
	cmp	r4, r0
	bge	.LA9	@cond_branch
	add	r5, r1, #0
.LA11:
	bl	VBlankIntrWait
	add	r4, r4, #1
	ldrb	r2, [r5, #14]
	cmp	r4, r2
	blt	.LA11	@cond_branch
.LA9:
	ldr	r4, .LA23+8
	ldrb	r5, [r4, #13]
	bl	STWI_init
	strb	r5, [r4, #13]
	mov	r0, #2
	strb	r0, [r4, #10]
	b	.LA4
.LA24:
	.align	2
.LA23:
	.word	67109152
	.word	STWI_buffer_send
	.word	STWI_status
	.word	67109160
	.word	20611
.LA7:
	lsl	r0, r2, #24
	lsr	r0, r0, #24
	cmp	r0, #3
	bne	.LA20	@cond_branch
	ldr	r1, .LA25
	mov	r3, #128
	lsl	r3, r3, #8
	add	r0, r3, #0
	strh	r0, [r1]
	ldr	r2, .LA25+4
	add	r0, r2, #0
	mov	r4, #250
	lsl	r4, r4, #2
.LA17:
	strh	r0, [r1]
	sub	r4, r4, #1
	cmp	r4, #0
	bne	.LA17	@cond_branch
	ldr	r1, .LA25
	mov	r3, #128
	lsl	r3, r3, #8
	add	r0, r3, #0
	strh	r0, [r1]
	mov	r0, #0
	strh	r0, [r1]
	sub	r1, r1, #12
	ldr	r2, .LA25+8
	add	r0, r2, #0
	strh	r0, [r1]
.LA20:
	mov	r0, #0
.LA19:
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.LA26:
	.align	2
.LA25:
	.word	67109172
	.word	33023
	.word	20483
.LAfe1:
	.size	 STWI_poll_CommandEnd,.LAfe1-STWI_poll_CommandEnd
");
#else

#include <Agb.h>

#include "STWI_status.h"
extern u32 STWI_init(void);
extern struct STWI_status STWI_status;
extern u8 STWI_buffer_send[0x120];

u16 STWI_poll_CommandEnd(void)
{
    int x;
    u8 tmp;
    u8 *ptr8;
    u16 val;

    if (!STWI_status.modeMaster) return 1;

retry:
    *(vu32 *)REG_SIODATA32 = *(u32 *)STWI_buffer_send;
    STWI_status.cmdHeader = *(u32 *)STWI_buffer_send;
    STWI_status.field3_0x9 = 1;
    *(vu16 *)REG_SIOCNT = 0x5083;

    ptr8 = &STWI_status.unk_07;
wait:
    if (*ptr8 == 0) goto wait;

    if ((u8)(STWI_status.unk_08 - 1) <= 1) {
        for (x = 0; x < STWI_status.unk_09; x++) VBlankIntrWait();

        tmp = STWI_status.unk_08;
        STWI_init();
        STWI_status.unk_08 = tmp;
        STWI_status.error = 2;
        goto retry;
    }

    if (STWI_status.unk_08 != 3) return 0;

    *(vu16 *)REG_RCNT = 0x8000;
    val = 0x80ff;
    for (x = 1000; x; x--) *(u16 *)REG_RCNT = val;
    *(vu16 *)REG_RCNT = 0x8000;
    val = 0;

    *(vu16 *)REG_RCNT = 0;
    *(vu16 *)REG_SIOCNT = 0x5003;

    return 0;
}

#endif
