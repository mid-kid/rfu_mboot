#if 1
__asm__("
.text
	.align	2
	.globl	RfuCmd_GameInfoSet
	.type	 RfuCmd_GameInfoSet,function
	.thumb_func
RfuCmd_GameInfoSet:
	push	{r4, r5, lr}
	add	r4, r0, #0
	add	r5, r1, #0
	bl	RfuCmdReset
	ldr	r0, .L22
	ldr	r1, .L22+4
	stmia	r0!, {r1}
	add	r2, r0, #0
	ldrh	r0, [r4]
	strh	r0, [r2]
	add	r2, r2, #2
	add	r4, r4, #2
	mov	r1, #13
.L6:
	ldrb	r0, [r4]
	strb	r0, [r2]
	add	r2, r2, #1
	add	r4, r4, #1
	sub	r1, r1, #1
	cmp	r1, #0
	bge	.L6	@cond_branch
	ldr	r3, .L22+8
	mov	r1, #0
	b	.L8
.L23:
	.align	2
.L22:
	.word	STWI_buffer_send
	.word	-1721367018
	.word	STWI_status
.L12:
	strb	r0, [r2]
	add	r2, r2, #1
	add	r5, r5, #1
	add	r1, r1, #1
	cmp	r1, #7
	bgt	.L9	@cond_branch
.L8:
	ldrb	r0, [r5]
	cmp	r0, #0
	bne	.L12	@cond_branch
	cmp	r1, #7
	bgt	.L9	@cond_branch
	mov	r0, #0
.L16:
	strb	r0, [r2]
	add	r2, r2, #1
	add	r1, r1, #1
	cmp	r1, #7
	ble	.L16	@cond_branch
.L9:
	mov	r0, #6
	strb	r0, [r3, #8]
	bl	RfuCmdSend
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r0, #1
	beq	.L19	@cond_branch
	ldr	r0, .L24
	mov	r1, #0
	bl	RfuCmdRecv
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	b	.L21
.L25:
	.align	2
.L24:
	.word	-1721368426
.L19:
	mov	r0, #5
.L21:
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.Lfe1:
	.size	 RfuCmd_GameInfoSet,.Lfe1-RfuCmd_GameInfoSet
");
#else

#include <Agb.h>

#include "STWI_status.h"
extern u32 RfuCmdReset(void);
extern u16 RfuCmdSend(void);
extern u16 RfuCmdRecv(u32 Cmd, u8 VarSize);
extern u8 STWI_buffer_send[0x120];
extern struct STWI_status STWI_status;

u16 RfuCmd_GameInfoSet(char *GameData, char *UserName)
{
    u8 *dst;
    u8 *src;
    int x;

    RfuCmdReset();

    dst = STWI_buffer_send;
    *((u32 *)dst)++ = 0x99660616;
    *((u16 *)dst)++ = *(u16 *)GameData;

    src = GameData + 2;
    for (x = 0; x < 14; x++) *dst++ = *src++;

    src = UserName;
    for (x = 0; x < 8; x++) {
        if (!*src) {
            for (; x < 8; x++) *dst++ = '\0';
            break;
        }
        *dst++ = *src++;
    }

    STWI_status.cmdSize = 6;
    if (RfuCmdSend() == 1) {
        return 5;
    } else {
        return RfuCmdRecv(0x99660096, FALSE);
    }
}
#endif
