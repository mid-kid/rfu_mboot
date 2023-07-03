#include <Agb.h>

u32 Sio32IntrMaster(void);
u32 Sio32IntrSlave(void);
u16 Sio32WaitSIState(u16 State);
void Call_thumb(void (*)());

#include "Rfu.h"
extern u8 u8_03005efc;
extern void (*Sio32IntrProc)(void);
extern struct Rfu Rfu;

void Sio32Intr(void)
{
    if (u8_03005efc == 1) {
        Call_thumb(Sio32IntrProc);
        return;
    }

    Rfu.unk_10 = 0;
    if (Rfu.modeMaster == TRUE) {
        Sio32IntrMaster();
    } else {
        Sio32IntrSlave();
    }
}

extern u8 RfuBufRecv[0x120];
extern u8 u8_03005729;
extern u8 u8_03005728;
extern u8 RfuBufSend[0x120];

#define BufWrite(Buf, Offs, Data, Bit) \
({ \
    u8 *buf = (u8 *)&Buf; \
    *(u##Bit *)(buf + (Offs)) = (Data); \
})

#define BufRead(Buf, Offs, Bit) \
({ \
    u8 *buf = (u8 *)&Buf[Offs]; \
    *(u##Bit *)(buf); \
})

#if 1
__asm__("
    .text
	.global	Sio32IntrMaster
	.type	 Sio32IntrMaster,function
Sio32IntrMaster:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, current_function_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}
	ldr	r3, .LL36
	sub	fp, ip, #4
	mov	lr, #288
	add	lr, lr, #67108864
	ldr	r0, [r3, #0]
	mov	r4, r3
	ldr	ip, [lr, #0]
	cmp	r0, #3
	bne	.LL9
	ldr	r2, .LL36+4
	ldr	r1, .LL36+8
	ldr	r0, .LL36+12
	ldrb	r3, [r2, #0]	@ zero_extendqisi2
	str	ip, [r1, r3, asl #2]
	ldrb	r3, [r2, #0]
	ldrb	r1, [r0, #0]
	add	r3, r3, #1
	strb	r3, [r2, #0]
	and	r3, r1, #255
	cmp	r3, #0
	streqb	r3, [r4, #10]
	moveq	r3, #4
	streq	r3, [r4, #0]
	beq	.LL12
.LL10:
	sub	r3, r1, #1
	strb	r3, [r0, #0]
	ldr	r2, [r4, #4]
	mov	r3, #-2147483648
	str	r3, [lr, #0]
	add	r2, r2, ip
	str	r2, [r4, #4]
	b	.LL12
.LL9:
	cmp	r0, #0
	bne	.LL13
	cmp	ip, #-2147483648
	beq	.LL14
	ldrb	r3, [r4, #13]
	mov	r2, #1
	strb	r2, [r4, #12]
	add	r3, r3, r2
	strb	r3, [r4, #13]
	b	.LL35
.LL14:
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	ldrb	r0, [r4, #9]	@ zero_extendqisi2
	add	r3, r3, #1
	cmp	r3, r0
	ble	.LL19
	ldr	ip, .LL36+16
	mov	r0, r0, asl #2
	ldr	r3, [ip, r0]
	str	r3, [lr, #0]
	ldr	r2, [r4, #4]
	ldrb	r3, [r4, #9]
	ldr	r1, [ip, r0]
	add	r3, r3, #1
	strb	r3, [r4, #9]
	add	r2, r2, r1
	str	r2, [r4, #4]
	b	.LL12
.LL13:
	cmp	r0, #1
	bne	.LL18
	cmp	ip, #-2147483648
	beq	.LL19
	ldrb	r3, [r4, #13]
	strb	r0, [r4, #12]
	add	r3, r3, #1
	strb	r3, [r4, #13]
	b	.LL34
.LL19:
	mov	r3, #2
	str	r3, [r4, #0]
	str	ip, [lr, #0]
	b	.LL12
.LL18:
	cmp	r0, #2
	bne	.LL12
	str	ip, [r4, #4]
	mov	r3, #39168
	ldr	r2, .LL36+8
	mov	r0, #0
	str	ip, [r2, r0]
	add	r3, r3, #102
	ldr	r2, .LL36+4
	add	r1, r0, #1
	strb	r1, [r2, #0]
	cmp	r3, ip, lsr #16
	bne	.LL22
	and	r3, ip, #65280
	ldr	r1, .LL36+12
	mov	r3, r3, lsr #8
	strb	r3, [r1, #0]
	cmp	r3, #69
	movhi	r3, #69
	strhib	r3, [r1, #0]
.LL23:
	ldrb	r3, [r1, #0]
	and	r2, r3, #255
	cmp	r2, #0
	streqb	r2, [r4, #10]
	moveq	r3, #4
	streq	r3, [r4, #0]
	beq	.LL26
.LL24:
	sub	r3, r3, #1
	strb	r3, [r1, #0]
	mov	r2, #3
	str	r2, [r4, #0]
	b	.LL26
.LL22:
	ldrb	r3, [r4, #13]
	strb	r1, [r4, #12]
	add	r3, r3, #1
	strb	r3, [r4, #13]
	b	.LL35
.LL26:
	mov	r3, #288
	add	r3, r3, #67108864
	mov	r2, #-2147483648
	str	r2, [r3, #0]
.LL12:
	mov	r0, #1
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.LL34
	mov	r5, #296
	add	r5, r5, #67108864
	mov	r6, #20480
	add	r3, r6, #11
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	mov	r0, #0
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.LL34
	ldr	r3, [r4, #0]
	cmp	r3, #4
	bne	.LL29
	mov	r3, #167
	add	r3, r3, #-1728053248
	add	r3, r3, #6684672
	mov	r2, #165
	ldr	r1, .LL36+8
	add	r2, r2, #-1728053248
	ldr	r1, [r1, #0]
	add	r2, r2, #6684672
	cmp	r1, r2
	cmpne	r1, r3
	beq	.LL31
	mov	r3, #181
	add	r3, r3, #-1728053248
	add	r3, r3, #6684672
	cmp	r1, r3
	beq	.LL31
	mov	r3, #183
	add	r3, r3, #-1728053248
	add	r3, r3, #6684672
	cmp	r1, r3
	bne	.LL30
.LL31:
	mov	r0, #288
	add	r0, r0, #67108864
	mov	ip, #296
	add	ip, ip, #67108864
	mov	r3, #0
	strb	r3, [r4, #11]
	mov	r1, #5
	str	r1, [r4, #0]
    mov	r2, #-2147483648
	str	r2, [r0, #0]
    add	r3, r3, #20480
	add	r2, r3, #2
	strh	r2, [ip, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	add	r3, r3, #130
	strh	r3, [ip, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	b	.LL32
.LL30:
	add	r3, r6, #3
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
.LL32:
	mov	r3, #0
	strb	r3, [r4, #13]
	mov	r2, #255
	strb	r2, [r4, #15]
	add	r3, r3, #1
	strb	r3, [r4, #12]
	b	.LL33
.LL37:
	.align	0
.LL36:
	.word	Rfu
	.word	u8_03005729
	.word	RfuBufRecv
	.word	u8_03005728
	.word	RfuBufSend
.LL29:
	add	r3, r6, #3
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
.LL33:
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.LL34
	mov	r2, #296
	add	r2, r2, #67108864
	mov	r3, #20480
	add	r3, r3, #131
	strh	r3, [r2, #0]	@ movhi   ;; CYGNUS LOCAL nickc
.LL34:
	mov	r0, #0
.LL35:
	ldmea	fp, {r4, r5, r6, fp, sp, lr}
	bx	lr
.LLfe2:
	.size	 Sio32IntrMaster,.LLfe2-Sio32IntrMaster
");
#else
u32 Sio32IntrMaster(void)
{
    u32 data;

    data = *(u32 *)REG_SIODATA32;

    if (Rfu.state == 3) {
        BufWrite(RfuBufRecv, u8_03005729 * 4, data, 32);
        u8_03005729++;

        if (u8_03005728 == 0) {
            Rfu.error = 0;
            Rfu.state = 4;
        } else {
            u8_03005728--;
            *(u32 *)REG_SIODATA32 = 0x80000000;
            Rfu.cmdHeader += data;
        }
    } else if (Rfu.state == 0) {
        if (data != 0x80000000) {
            Rfu.unk_07 = 1;
            Rfu.unk_08++;
            return 0;
        }

        if (Rfu.cmdSize + 1 > Rfu.field3_0x9) {
            *(vu32 *)REG_SIODATA32 = BufRead(RfuBufSend, Rfu.field3_0x9 * 4, 32);
            Rfu.cmdHeader += BufRead(RfuBufSend, Rfu.field3_0x9 * 4, 32);
            Rfu.field3_0x9++;
        } else {
            Rfu.state = 2;
            *(u32 *)REG_SIODATA32 = data;
        }
    } else if (Rfu.state == 1) {
        if (data != 0x80000000) {
            Rfu.unk_07 = 1;
            Rfu.unk_08++;
            return 0;
        }

        Rfu.state = 2;
        *(u32 *)REG_SIODATA32 = data;
    } else if (Rfu.state == 2) {
        Rfu.cmdHeader = data;
        u8_03005729 = 0;

        BufWrite(RfuBufRecv, u8_03005729, data, 32);
        u8_03005729++;

        if (data >> 0x10 == 0x9966) {
            u8_03005728 = (data & 0xff00) >> 8;
            if (u8_03005728 > 0x45) u8_03005728 = 0x45;

            if (u8_03005728 == 0) {
                Rfu.error = 0;
                Rfu.state = 4;
            } else {
                u8_03005728--;
                Rfu.state = 3;
            }
        } else {
            Rfu.unk_07 = 1;
            Rfu.unk_08++;
            return 0;
        }

        *(vu32 *)REG_SIODATA32 = 0x80000000;
    }

    if (Sio32WaitSIState(1) == 1) return 0;
    *(vu16 *)REG_SIOCNT = 0x500b;
    if (Sio32WaitSIState(0) == 1) return 0;

    if (Rfu.state == 4) {
        if (*(u32 *)RfuBufRecv == 0x996600a7 ||
                *(u32 *)RfuBufRecv == 0x996600a5 ||
                *(u32 *)RfuBufRecv == 0x996600b5 ||
                *(u32 *)RfuBufRecv == 0x996600b7) {
            Rfu.modeMaster = FALSE;
            *(vu32 *)REG_SIODATA32 = 0x80000000;
            *(vu16 *)REG_SIOCNT = 0x5002;
            *(vu16 *)REG_SIOCNT = 0x5082;
            Rfu.state = 5;
        } else {
            *(vu16 *)REG_SIOCNT = 0x5003;
        }

        Rfu.unk_08 = 0;
        Rfu.unk_10 = 0xff;
        Rfu.unk_07 = 1;
    } else {
        *(vu16 *)REG_SIOCNT = 0x5003;
    }

    if (Rfu.unk_07 == 0) {
        *(vu16 *)REG_SIOCNT = 0x5083;
    }

    return 0;
}
#endif

extern u8 RfuIntrCmd;
extern u8 RfuIntrSize;

#if 1
__asm__("
	.text
	.global	Sio32IntrSlave
	.type	 Sio32IntrSlave,function
Sio32IntrSlave:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, current_function_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}
	mov	r0, #0
	sub	fp, ip, #4
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.L31
	mov	r1, #296
	add	r1, r1, #67108864
	mov	r2, #20480
	add	r2, r2, #10
	ldr	r0, .L34
	mov	r3, #288
	strh	r2, [r1, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	add	r3, r3, #67108864
	ldr	lr, [r0, #0]
	mov	r4, r0
	ldr	ip, [r3, #0]
	cmp	lr, #5
	bne	.L10
	mov	r3, #0
	add	r5, r3, #1
	strb	r5, [r4, #9]
	str	ip, [r4, #4]
	mov	r2, #39168
	ldr	r1, .L34+4
	add	r2, r2, #102
	str	ip, [r1, r3]
	cmp	r2, ip, lsr #16
	bne	.L11
	and	r3, ip, #65280
	mov	r3, r3, lsr #8
	strb	r3, [r4, #8]
	ldr	r0, .L34+8
	ldr	r2, .L34+12
	strb	r3, [r0, #0]
	strb	ip, [r2, #0]
    ands	r1, r3, #255
	bne	.L12
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	sub	r3, r2, #39
	cmp	r2, #54
	cmpne	r3, #2
	bhi	.L13
	strb	r1, [r4, #8]
	add	r3, r2, #128
	ldr	r2, .L34+16
	add	r3, r3, #-1728053248
	strb	r1, [r0, #0]
	add	r3, r3, #6684672
	str	r3, [r2, #0]
	b	.L33
.L13:
	strb	r5, [r4, #8]
	ldr	r3, .L34+16
	ldr	r2, .L34+20
	strb	r5, [r0, #0]
	str	r2, [r3, #0]
	mov	r1, r3
	str	r5, [r3, #4]
	b	.L21
.L12:
	sub	r3, r3, #1
	strb	r3, [r0, #0]
	mov	r2, #6
	str	r2, [r4, #0]
	b	.L24
.L11:
	str	lr, [r4, #0]
	b	.L24
.L10:
	cmp	lr, #6
	bne	.L18
	ldr	r2, .L34+4
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	ldr	r0, .L34+8
	str	ip, [r2, r3, asl #2]
	ldrb	r3, [r4, #9]
	ldrb	r2, [r0, #0]
	add	r3, r3, #1
	and	r1, r2, #255
	strb	r3, [r4, #9]
	cmp	r1, #0
	bne	.L19
	ldr	r3, .L34+12
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	sub	r2, r3, #40
	cmp	r3, #54
	cmpne	r2, #1
	bhi	.L20
	strb	r1, [r4, #8]
    add	r3, r3, #128
	ldr	r2, .L34+16
	add	r3, r3, #-1728053248
	strb	r1, [r0, #0]
	add	r3, r3, #6684672
	str	r3, [r2, #0]
	b	.L33
.L20:
	mov	r3, #1
	strb	r3, [r4, #8]
	ldr	r2, .L34+16
	ldr	r1, .L34+20
	strb	r3, [r0, #0]
	stmia	r2, {r1, r3}	@ phole stm
.L33:
	mov	r1, r2
.L21:
	mov	r2, #0
	strb	r2, [r4, #9]
	mov	r3, #7
	str	r3, [r4, #0]
	ldr	r1, [r1, r2]
	str	r1, [r4, #4]
	add	r2, r2, #1
	strb	r2, [r4, #9]
	b	.L17
.L19:
	ldr	r3, [r4, #4]
	sub	r2, r2, #1
	strb	r2, [r0, #0]
	add	r3, r3, ip
	str	r3, [r4, #4]
	b	.L24
.L18:
	cmp	lr, #7
	bne	.L24
	ldr	r2, .L34+8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq	.L25
	sub	r3, r3, #1
	strb	r3, [r2, #0]
	ldr	r1, .L34+16
	ldrb	r2, [r4, #9]
    ldrb	r3, [r4, #9]
	ldr	r1, [r1, r2, asl #2]
	ldr	r2, [r4, #4]
	add	r3, r3, #1
	strb	r3, [r4, #9]
	add	r2, r2, r1
	str	r2, [r4, #4]
	b	.L17
.L25:
	mov	r3, #9
	str	r3, [r4, #0]
	ldr	r1, .L34+16
	ldrb	r2, [r4, #9]	@ zero_extendqisi2
	ldr	r3, [r4, #4]
	str	r3, [r1, r2, asl #2]
.L24:
	mov	r1, #-2147483648
.L17:
	mov	r6, #288
	add	r6, r6, #67108864
	str	r1, [r6, #0]
	mov	r0, #1
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.L31
	mov	r5, #296
	add	r5, r5, #67108864
	mov	r7, #20480
	ldr	r2, [r4, #0]
	add	r3, r7, #2
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	cmp	r2, #9
	bne	.L29
	mov	r0, #0
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.L31
	mov	r1, #0
	str	r1, [r6, #0]
    str	r1, [r4, #0]
	mov	r3, #1
	strb	r3, [r4, #11]
	mov	r2, #255
	strb	r2, [r4, #15]
    mov	r3, r1
	strb	r3, [r4, #13]
	strh	r1, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	ldr	r3, .L34+24
	ldr	r0, [r3, #0]
	add	r2, r7, #3
	strh	r2, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	bl	Call_thumb
	b	.L31
.L35:
	.align	0
.L34:
	.word	Rfu
	.word	RfuBufRecv
	.word	RfuIntrSize
	.word	RfuIntrCmd
	.word	RfuBufSend
	.word	-1721368082
	.word	Sio32IntrProc
.L29:
	mov	r3, #20480
	add	r3, r3, #130
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
.L31:
	mov	r0, #0
	ldmea	fp, {r4, r5, r6, r7, fp, sp, lr}
	bx	lr
.Lfe2:
	.size	 Sio32IntrSlave,.Lfe2-Sio32IntrSlave
");
#else
u32 Sio32IntrSlave(void)
{
    u32 data;
    u32 data_send;

    if (Sio32WaitSIState(0) == 1) return 0;
    *(vu16 *)REG_SIOCNT = 0x500a;

    data = *(vu32 *)REG_SIODATA32;

    if (Rfu.state == 5) {
        Rfu.field3_0x9 = 0;
        BufWrite(RfuBufRecv, Rfu.field3_0x9, data, 32);
        Rfu.field3_0x9++;

        Rfu.cmdHeader = data;

        if (data >> 0x10 == 0x9966) {
            Rfu.cmdSize = (data & 0xff00) >> 8;
            RfuIntrSize = Rfu.cmdSize;
            RfuIntrCmd = data;

            if (Rfu.cmdSize == 0) {
                if (RfuIntrCmd == 0x27 ||
                        RfuIntrCmd == 0x28 ||
                        RfuIntrCmd == 0x29 ||
                        RfuIntrCmd == 0x36) {
                    Rfu.cmdSize = 0;
                    RfuIntrSize = Rfu.cmdSize;
                    *(u32 *)RfuBufSend = 0x99660080 + RfuIntrCmd;
                } else {
                    Rfu.cmdSize = 1;
                    RfuIntrSize = Rfu.cmdSize;
                    BufWrite(RfuBufSend, 0, 0x996601ee, 32);
                    BufWrite(RfuBufSend, 4, 0x00000001, 32);
                }

                Rfu.field3_0x9 = 0;
                Rfu.state = 7;
                data_send = BufRead(RfuBufSend, Rfu.field3_0x9, 32);
                Rfu.cmdHeader = data_send;
                Rfu.field3_0x9++;

            } else {
                RfuIntrSize = Rfu.cmdSize - 1;
                Rfu.state = 6;
                data_send = 0x80000000;
            }
        } else {
            Rfu.state = 5;
            data_send = 0x80000000;
        }
    } else if (Rfu.state == 6) {
        BufWrite(RfuBufRecv, Rfu.field3_0x9 * 4, data, 32);
        Rfu.field3_0x9++;

        if (RfuIntrSize == 0) {
            if (RfuIntrCmd == 0x28 ||
                    RfuIntrCmd == 0x29 ||
                    RfuIntrCmd == 0x36) {
                Rfu.cmdSize = 0;
                RfuIntrSize = Rfu.cmdSize;
                *(u32 *)RfuBufSend = 0x99660080 + RfuIntrCmd;
            } else {
                Rfu.cmdSize = 1;
                RfuIntrSize = Rfu.cmdSize;
                BufWrite(RfuBufSend, 0, 0x996601ee, 32);
                BufWrite(RfuBufSend, 4, 0x00000001, 32);
            }
            Rfu.field3_0x9 = 0;
            Rfu.state = 7;
            data_send = BufRead(RfuBufSend, Rfu.field3_0x9, 32);
            Rfu.cmdHeader = data_send;
            Rfu.field3_0x9++;
        } else {
            RfuIntrSize--;
            Rfu.cmdHeader += data;
            data_send = 0x80000000;
        }
    } else if (Rfu.state == 7) {
        if (RfuIntrSize != 0) {
            RfuIntrSize--;
            data_send = BufRead(RfuBufSend, Rfu.field3_0x9 * 4, 32);
            Rfu.cmdHeader += data_send;
            Rfu.field3_0x9++;
        } else {
            Rfu.state = 9;
            BufWrite(RfuBufSend, Rfu.field3_0x9 * 4, Rfu.cmdHeader, 32);
            data_send = 0x80000000;
        }
    } else {
        data_send = 0x80000000;
    }
    *(vu32 *)REG_SIODATA32 = data_send;

    if (Sio32WaitSIState(1) == 1) return 0;

    *(vu16 *)REG_SIOCNT = 0x5002;

    if (Rfu.state == 9) {
        if (Sio32WaitSIState(0) == 1) return 0;

        *(vu32 *)REG_SIODATA32 = 0;
        Rfu.state = 0;
        Rfu.modeMaster = TRUE;
        Rfu.unk_10 = 0xff;
        Rfu.unk_08 = 0;
        *(vu16 *)REG_SIOCNT = 0;
        *(vu16 *)REG_SIOCNT = 0x5003;
        Call_thumb(Sio32IntrProc);
    } else {
        *(vu16 *)REG_SIOCNT = 0x5082;
    }

    return 0;
}
#endif

#if 1
__asm__("
.section .text
.type Sio32WaitSIState, function
Sio32WaitSIState:
.2byte 0xc00d,0xe1a0,0xd800,0xe92d,0xb004,0xe24c,0xd00c,0xe24d,0x3000,0xe1a0,0x30be,0xe14b,0x3000,0xe3a0,0x3018,0xe50b,0x3018,0xe51b,0x2001,0xe283,0x2018,0xe50b,0x307c,0xe59f,0x2011,0xe5d3,0x0001,0xe352,0x000c,0x1a00,0x3018,0xe51b,0x0b26,0xe353,0x0009,0x1a00,0x3060,0xe59f,0x2003,0xe3a0,0x1002,0xe1a0,0x100a,0xe5c3,0x3050,0xe59f,0x2001,0xe3a0,0x1002,0xe1a0,0x100c,0xe5c3,0x0001,0xe3a0,0x0010,0xea00,0x3f4a,0xe3a0,0x3301,0xe283,0x20b0,0xe1d3,0x1802,0xe1a0,0x3821,0xe1a0,0x3014,0xe50b,0x2014,0xe51b,0x3004,0xe202,0x20be,0xe15b,0x1102,0xe1a0,0x0001,0xe153,0x0000,0x1a00,0x0000,0xea00,0xffdd,0xeaff,0x0000,0xe3a0,0x0000,0xea00,0x5ca0,0x0300,0x6800,0xe91b,0xff1e,0xe12f
.size Sio32WaitSIState, .-Sio32WaitSIState
");
#else
#endif

__asm__("
Call_thumb:
    bx r0
");
