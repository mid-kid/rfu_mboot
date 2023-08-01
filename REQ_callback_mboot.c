#ifndef NONMATCHING
__asm__("
.text
	.align	2
	.globl	REQ_callback_mboot
	.type	 REQ_callback_mboot,function
	.thumb_func
REQ_callback_mboot:
	push	{r4, r5, r6, lr}
	add	sp, sp, #-4
	mov	r4, #0
	ldr	r0, .L15
	strb	r4, [r0]
	mov	r2, sp
	add	r2, r2, #1
	mov	r0, sp
	add	r1, r2, #0
	bl	rfu_REQBN_watchLink
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
	.word	rfuSlotStatus_NI
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
	.word	my_state
.L21:
	ldr	r5, .L19
	ldrb	r0, [r5]
	cmp	r0, #197
	bhi	.L11	@cond_branch
	bl	rfu_REQ_recvData
	bl	rfu_REQ_sendData
.L11:
	ldr	r6, .L19+4
.L14:
	bl	rfu_REQ_changeMasterSlave
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
	.word	my_state
	.word	SearchMenuErrorMsg
.Lfe1:
	.size	 REQ_callback_mboot,.Lfe1-REQ_callback_mboot
");
#else

#include <Agb.h>

#include "AgbRFU_LL.h"
extern u32 rfu_REQBN_watchLink(u8 *param_1, u8 *param_2, u8 *param_3);
extern u16 rfu_REQ_recvData(void);
extern u16 rfu_REQ_sendData(void);
extern u16 rfu_REQ_changeMasterSlave(void);
extern u8 SearchMenuErrorTimer;
extern u8 my_state;
extern u8 SearchMenuErrorMsg;
extern u8 MbootPeer;

void REQ_callback_mboot(void)
{
    u8 x;
    u8 res;
    u8 res2;
    u8 res3;

    x = 0;
    SearchMenuErrorTimer = 0;

    rfu_REQBN_watchLink(&res, &res2, &res3);
    if (res != 0) {
        if (rfuSlotStatus_NI[MbootPeer].recv.state != 0) {
            SearchMenuErrorMsg = 1;
        } else {
            SearchMenuErrorMsg = 3;
        }
    } else {
        if (my_state < 0xc6) {
            rfu_REQ_recvData();
            rfu_REQ_sendData();
        }

        for (x = 0; x != 2; x++) {
            if (rfu_REQ_changeMasterSlave() == 0) return;
        }
        SearchMenuErrorMsg = 4;
    }

    my_state = 0xc1;
}
#endif
