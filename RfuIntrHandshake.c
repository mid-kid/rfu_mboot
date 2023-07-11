static const char HandshakeString[] = {'N', 'I', 'N', 'T', 'E', 'N', 'D', 'O'};

#if 1
__asm__("
.text
	.align	2
	.globl	RfuIntrHandshake
	.type	 RfuIntrHandshake,function
	.thumb_func
RfuIntrHandshake:
	push	{r4, r5, lr}
	ldr	r0, .L14
	ldr	r3, [r0]
	ldr	r0, .L14+4
	ldrb	r1, [r0]
	add	r4, r0, #0
	cmp	r1, #1
	beq	.L3	@cond_branch
	ldr	r0, .L14+8
	ldrh	r1, [r0]
	mov	r2, #128
	orr	r1, r1, r2
	strh	r1, [r0]
.L3:
	ldrb	r2, [r4]
	lsl	r1, r2, #4
	add	r0, r3, #0
	lsl	r0, r0, r1
	lsr	r1, r0, #16
	mov	r0, #1
	sub	r0, r0, r2
	lsl	r0, r0, #4
	lsl	r3, r3, r0
	add	r0, r3, #0
	lsr	r3, r0, #16
	ldrh	r0, [r4, #10]
	cmp	r0, #0
	bne	.L4	@cond_branch
	ldrh	r2, [r4, #6]
	cmp	r1, r2
	bne	.L5	@cond_branch
	ldrh	r5, [r4, #2]
	cmp	r5, #3
	bls	.L6	@cond_branch
	strh	r3, [r4, #10]
	b	.L4
.L15:
	.align	2
.L14:
	.word	67109152
	.word	Sio32
	.word	67109160
.L6:
	ldrh	r0, [r4, #4]
	mvn	r0, r0
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r1, r0
	bne	.L4	@cond_branch
	mvn	r0, r2
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r3, r0
	bne	.L4	@cond_branch
	add	r0, r5, #1
.L5:
	strh	r0, [r4, #2]
.L4:
	ldrh	r0, [r4, #2]
	cmp	r0, #3
	bhi	.L10	@cond_branch
	lsl	r0, r0, #1
	ldr	r1, .L16
	add	r0, r0, r1
	ldrh	r0, [r0]
	b	.L13
.L17:
	.align	2
.L16:
	.word	HandshakeString
.L10:
	ldr	r0, .L18
.L13:
	strh	r0, [r4, #4]
	mvn	r0, r3
	strh	r0, [r4, #6]
	ldr	r3, .L18+4
	ldrh	r2, [r4, #4]
	ldrb	r1, [r4]
	mov	r0, #1
	sub	r0, r0, r1
	lsl	r0, r0, #4
	lsl	r2, r2, r0
	ldrh	r0, [r4, #6]
	lsl	r1, r1, #4
	lsl	r0, r0, r1
	add	r2, r2, r0
	str	r2, [r3]
	ldrb	r0, [r4]
	cmp	r0, #1
	bne	.L12	@cond_branch
	ldrh	r0, [r4, #10]
	cmp	r0, #0
	bne	.L12	@cond_branch
	ldr	r0, .L18+8
	ldrh	r1, [r0]
	mov	r2, #128
	orr	r1, r1, r2
	strh	r1, [r0]
.L12:
	pop	{r4, r5}
	pop	{r0}
	bx	r0
.L19:
	.align	2
.L18:
	.word	32769
	.word	67109152
	.word	67109160
.Lfe1:
	.size	 RfuIntrHandshake,.Lfe1-RfuIntrHandshake
");
#else

#include <Agb.h>

#include "Sio32.h"
extern struct Sio32 Sio32;

void RfuIntrHandshake(void)
{
    u32 data;
    u32 DataHi;
    u32 DataLo;

    data = *(vu32 *)REG_SIODATA32;
    if (Sio32.slave != TRUE) {
        *(vu16 *)REG_SIOCNT |= 0x80;
    }

    DataHi = data << ((0 + Sio32.slave) * 16) >> 16;
    DataLo = data << ((1 - Sio32.slave) * 16) >> 16;

    if (Sio32.deviceID == 0) {
        if (DataHi == Sio32.dataHi) {
            if (Sio32.handshakeStep < 4) {
                if ((u16)~Sio32.dataLo == DataHi && (u16)~Sio32.dataHi == DataLo) {
                    Sio32.handshakeStep++;
                }
            } else {
                Sio32.deviceID = DataLo;
            }
        } else {
            Sio32.handshakeStep = 0;
        }
    }
    if (Sio32.handshakeStep < 4) {
        Sio32.dataLo = *(u16 *)(HandshakeString + Sio32.handshakeStep * 2);
    } else {
        // Send device ID
        Sio32.dataLo = 0x8001;
    }
    Sio32.dataHi = ~DataLo;

    *(vu32 *)REG_SIODATA32 =
        (Sio32.dataLo << ((1 - Sio32.slave) * 0x10)) +
        (Sio32.dataHi << ((0 + Sio32.slave) * 0x10));

    if (Sio32.slave == TRUE && Sio32.deviceID == 0) {
        *(vu16 *)REG_SIOCNT |= 0x80;
    }
}
#endif
