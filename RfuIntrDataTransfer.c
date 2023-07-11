#if 1
__asm__("
.text
	.align	2
	.globl	RfuIntrDataTransfer
	.type	 RfuIntrDataTransfer,function
	.thumb_func
RfuIntrDataTransfer:
	push	{r4, r5, r6, lr}
	add	sp, sp, #-4
	mov	r4, #0
	ldr	r0, .L15
	strb	r4, [r0]
	mov	r2, sp
	add	r2, r2, #1
	mov	r0, sp
	add	r1, r2, #0
	bl	RfuStatus
	mov	r0, sp
	ldrb	r0, [r0]
	cmp	r0, #0
	beq	.L21	@cond_branch
	ldr	r2, .L15+4
	ldr	r0, .L15+8
	ldrb	r1, [r0]
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	add	r0, r0, r2
	ldrh	r0, [r0, #52]
	cmp	r0, #0
	beq	.L4	@cond_branch
	ldr	r1, .L15+12
	mov	r0, #1
	b	.L3
.L16:
	.align	2
.L15:
	.word	SearchMenuErrorTimer
	.word	RfuPeers
	.word	MbootPeer
	.word	SearchMenuErrorMsg
.L4:
	ldr	r1, .L17
	mov	r0, #3
.L3:
	strb	r0, [r1]
	ldr	r1, .L17+4
	mov	r0, #193
	strb	r0, [r1]
	b	.L2
.L18:
	.align	2
.L17:
	.word	SearchMenuErrorMsg
	.word	MenuState
.L21:
	ldr	r5, .L19
	ldrb	r0, [r5]
	cmp	r0, #197
	bhi	.L11	@cond_branch
	bl	RfuDataRecv
	bl	RfuDataSend
.L11:
	ldr	r6, .L19+4
.L14:
	bl	RfuWaitDataStartForce
	lsl	r0, r0, #16
	cmp	r0, #0
	beq	.L2	@cond_branch
	add	r0, r4, #1
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, #2
	bne	.L14	@cond_branch
	mov	r0, #4
	strb	r0, [r6]
	mov	r0, #193
	strb	r0, [r5]
.L2:
	add	sp, sp, #4
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L20:
	.align	2
.L19:
	.word	MenuState
	.word	SearchMenuErrorMsg
.Lfe1:
	.size	 RfuIntrDataTransfer,.Lfe1-RfuIntrDataTransfer
");
#else

#include <Agb.h>

#include "RfuPeer.h"
extern u32 RfuStatus(u8 *param_1, u8 *param_2, u8 *param_3);
extern u16 RfuDataRecv(void);
extern u16 RfuDataSend(void);
extern u16 RfuWaitDataStartForce(void);
extern u8 SearchMenuErrorTimer;
extern u8 MenuState;
extern u8 SearchMenuErrorMsg;
extern struct RfuPeer RfuPeers[4];
extern u8 MbootPeer;

void RfuIntrDataTransfer(void)
{
    u8 x;
    u8 res;
    u8 res2;
    u8 res3;

    x = 0;
    SearchMenuErrorTimer = 0;

    RfuStatus(&res, &res2, &res3);
    if (res != 0) {
        if (RfuPeers[MbootPeer].sub[1].unk_01[0] != 0) {
            SearchMenuErrorMsg = 1;
        } else {
            SearchMenuErrorMsg = 3;
        }
    } else {
        if (MenuState < 0xc6) {
            RfuDataRecv();
            RfuDataSend();
        }

        for (x = 0; x != 2; x++) {
            if (RfuWaitDataStartForce() == 0) return;
        }
        SearchMenuErrorMsg = 4;
    }

    MenuState = 0xc1;
}
#endif
