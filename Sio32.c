#include <Agb.h>

u32 Sio32IntrMaster(void);
u32 Sio32IntrSlave(void);
u16 Sio32WaitSIState(u32 State);
void Call_thumb(void (*)());

#include "Rfu.h"
extern u8 u8_03005efc;
extern void (*Sio32IntrProc)(void);
extern struct Rfu Rfu;
extern u8 RfuBufRecv[0x120];
extern u8 u8_03005729;
extern u8 u8_03005728;
extern u8 RfuBufSend[0x120];

#define BufWrite(Buf, Offs, Data, Bit) \
{ \
    u8 *buf = (u8 *)&(Buf); \
    *(u##Bit *)(buf + (Offs)) = (Data); \
}

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
    u32 *buf;

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
            buf = &((u32 *)RfuBufSend)[Rfu.field3_0x9];
            *(u32 *)REG_SIODATA32 = *buf;
            Rfu.cmdHeader += *buf;
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

        *(u32 *)REG_SIODATA32 = 0x80000000;
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
	beq	.LY31
	mov	r1, #296
	add	r1, r1, #67108864
	mov	r2, #20480
	add	r2, r2, #10
	ldr	r0, .LY34
	mov	r3, #288
	strh	r2, [r1, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	add	r3, r3, #67108864
	ldr	lr, [r0, #0]
	mov	r4, r0
	ldr	ip, [r3, #0]
	cmp	lr, #5
	bne	.LY10
	mov	r3, #0
	add	r5, r3, #1
	strb	r5, [r4, #9]
	str	ip, [r4, #4]
	mov	r2, #39168
	ldr	r1, .LY34+4
	add	r2, r2, #102
	str	ip, [r1, r3]
	cmp	r2, ip, lsr #16
	bne	.LY11
	and	r3, ip, #65280
	mov	r3, r3, lsr #8
	strb	r3, [r4, #8]
	ldr	r0, .LY34+8
	ldr	r2, .LY34+12
	strb	r3, [r0, #0]
	strb	ip, [r2, #0]
    ands	r1, r3, #255
	bne	.LY12
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	sub	r3, r2, #39
	cmp	r2, #54
	cmpne	r3, #2
	bhi	.LY13
	strb	r1, [r4, #8]
	add	r3, r2, #128
	ldr	r2, .LY34+16
	add	r3, r3, #-1728053248
	strb	r1, [r0, #0]
	add	r3, r3, #6684672
	str	r3, [r2, #0]
	b	.LY33
.LY13:
	strb	r5, [r4, #8]
	ldr	r3, .LY34+16
	ldr	r2, .LY34+20
	strb	r5, [r0, #0]
	str	r2, [r3, #0]
	mov	r1, r3
	str	r5, [r3, #4]
	b	.LY21
.LY12:
	sub	r3, r3, #1
	strb	r3, [r0, #0]
	mov	r2, #6
	str	r2, [r4, #0]
	b	.LY24
.LY11:
	str	lr, [r4, #0]
	b	.LY24
.LY10:
	cmp	lr, #6
	bne	.LY18
	ldr	r2, .LY34+4
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	ldr	r0, .LY34+8
	str	ip, [r2, r3, asl #2]
	ldrb	r3, [r4, #9]
	ldrb	r2, [r0, #0]
	add	r3, r3, #1
	and	r1, r2, #255
	strb	r3, [r4, #9]
	cmp	r1, #0
	bne	.LY19
	ldr	r3, .LY34+12
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	sub	r2, r3, #40
	cmp	r3, #54
	cmpne	r2, #1
	bhi	.LY20
	strb	r1, [r4, #8]
    add	r3, r3, #128
	ldr	r2, .LY34+16
	add	r3, r3, #-1728053248
	strb	r1, [r0, #0]
	add	r3, r3, #6684672
	str	r3, [r2, #0]
	b	.LY33
.LY20:
	mov	r3, #1
	strb	r3, [r4, #8]
	ldr	r2, .LY34+16
	ldr	r1, .LY34+20
	strb	r3, [r0, #0]
	stmia	r2, {r1, r3}	@ phole stm
.LY33:
	mov	r1, r2
.LY21:
	mov	r2, #0
	strb	r2, [r4, #9]
	mov	r3, #7
	str	r3, [r4, #0]
	ldr	r1, [r1, r2]
	str	r1, [r4, #4]
	add	r2, r2, #1
	strb	r2, [r4, #9]
	b	.LY17
.LY19:
	ldr	r3, [r4, #4]
	sub	r2, r2, #1
	strb	r2, [r0, #0]
	add	r3, r3, ip
	str	r3, [r4, #4]
	b	.LY24
.LY18:
	cmp	lr, #7
	bne	.LY24
	ldr	r2, .LY34+8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq	.LY25
	sub	r3, r3, #1
	strb	r3, [r2, #0]
	ldr	r1, .LY34+16
	ldrb	r2, [r4, #9]
    ldrb	r3, [r4, #9]
	ldr	r1, [r1, r2, asl #2]
	ldr	r2, [r4, #4]
	add	r3, r3, #1
	strb	r3, [r4, #9]
	add	r2, r2, r1
	str	r2, [r4, #4]
	b	.LY17
.LY25:
	mov	r3, #9
	str	r3, [r4, #0]
	ldr	r1, .LY34+16
	ldrb	r2, [r4, #9]	@ zero_extendqisi2
	ldr	r3, [r4, #4]
	str	r3, [r1, r2, asl #2]
.LY24:
	mov	r1, #-2147483648
.LY17:
	mov	r6, #288
	add	r6, r6, #67108864
	str	r1, [r6, #0]
	mov	r0, #1
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.LY31
	mov	r5, #296
	add	r5, r5, #67108864
	mov	r7, #20480
	ldr	r2, [r4, #0]
	add	r3, r7, #2
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	cmp	r2, #9
	bne	.LY29
	mov	r0, #0
	bl	Sio32WaitSIState
	mov	r0, r0, asl #16
	cmp	r0, #65536
	beq	.LY31
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
	ldr	r3, .LY34+24
	ldr	r0, [r3, #0]
	add	r2, r7, #3
	strh	r2, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
	bl	Call_thumb
	b	.LY31
.LY35:
	.align	0
.LY34:
	.word	Rfu
	.word	RfuBufRecv
	.word	RfuIntrSize
	.word	RfuIntrCmd
	.word	RfuBufSend
	.word	-1721368082
	.word	Sio32IntrProc
.LY29:
	mov	r3, #20480
	add	r3, r3, #130
	strh	r3, [r5, #0]	@ movhi   ;; CYGNUS LOCAL nickc
.LY31:
	mov	r0, #0
	ldmea	fp, {r4, r5, r6, r7, fp, sp, lr}
	bx	lr
.LYfe2:
	.size	 Sio32IntrSlave,.LYfe2-Sio32IntrSlave
");
#else
u32 Sio32IntrSlave(void)
{
    u32 data;
    u32 data_send;
    u16 intrsize;
    u32 *buf;

    if (Sio32WaitSIState(0) == 1) return 0;
    *(vu16 *)REG_SIOCNT = 0x500a;

    data = *(vu32 *)REG_SIODATA32;

    if (Rfu.state == 5) {
        Rfu.field3_0x9 = 0;
        BufWrite(RfuBufRecv, Rfu.field3_0x9, data, 32);
        Rfu.field3_0x9++;

        Rfu.cmdHeader = data;

        if (data >> 0x10 == 0x9966) {
            intrsize = (data & 0xff00) >> 8;
            Rfu.cmdSize = intrsize;
            RfuIntrSize = Rfu.cmdSize;
            RfuIntrCmd = data;

            if (intrsize == 0) {
                u8 cmd = RfuIntrCmd;
                if (cmd == 0x27 || cmd == 0x28 || cmd == 0x29 || cmd == 0x36) {
                    Rfu.cmdSize = 0;
                    RfuIntrSize = Rfu.cmdSize;
                    BufWrite(RfuBufSend, 0, 0x99660080 + cmd, 32);
                } else {
                    Rfu.cmdSize = 1;
                    RfuIntrSize = Rfu.cmdSize;
                    BufWrite(RfuBufSend, 0, 0x996601ee, 32);
                    BufWrite(RfuBufSend, 4, 0x00000001, 32);
                }

                Rfu.field3_0x9 = 0;
                Rfu.state = 7;
                data_send = *(u32 *)RfuBufSend;
                Rfu.cmdHeader = *(u32 *)RfuBufSend;
                Rfu.field3_0x9++;

            } else {
                RfuIntrSize--;
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
            u8 cmd = RfuIntrCmd;
            if (cmd == 0x28 || cmd == 0x29 || cmd == 0x36) {
                Rfu.cmdSize = 0;
                RfuIntrSize = Rfu.cmdSize;
                *(u32 *)RfuBufSend = 0x99660080 + cmd;
            } else {
                Rfu.cmdSize = 1;
                RfuIntrSize = Rfu.cmdSize;
                BufWrite(RfuBufSend, 0, 0x996601ee, 32);
                BufWrite(RfuBufSend, 4, 0x00000001, 32);
            }
            Rfu.field3_0x9 = 0;
            Rfu.state = 7;
            data_send = *(u32 *)RfuBufSend;
            Rfu.cmdHeader = *(u32 *)RfuBufSend;
            Rfu.field3_0x9++;
        } else {
            RfuIntrSize--;
            Rfu.cmdHeader += data;
            data_send = 0x80000000;
        }
    } else if (Rfu.state == 7) {
        if (RfuIntrSize != 0) {
            RfuIntrSize--;
            buf = (u32 *)RfuBufSend;
            data_send = buf[Rfu.field3_0x9];
            Rfu.cmdHeader += buf[Rfu.field3_0x9];
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

        Rfu.state = 0;
        *(vu32 *)REG_SIODATA32 = 0;
        *(vu16 *)REG_SIOCNT = 0;
        Rfu.modeMaster = TRUE;
        Rfu.unk_10 = 0xff;
        Rfu.unk_08 = 0;
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
.text
.type Sio32WaitSIState, function
Sio32WaitSIState:
    mov	ip, sp
    stmdb	sp!, {fp, ip, lr, pc}
    sub	fp, ip, #4
    sub	sp, sp, #12
    mov	r3, r0
    strh	r3, [fp, #-14]
    mov	r3, #0
    str	r3, [fp, #-24]
5:
    ldr	r3, [fp, #-24]
    add	r2, r3, #1
    str	r2, [fp, #-24]
    ldr	r3, .LRfu
    ldrb	r2, [r3, #17]
    cmp	r2, #1
    bne	1f
    ldr	r3, [fp, #-24]
    cmp	r3, #38912
    bne	1f
    ldr	r3, .LRfu
    mov	r2, #3
    mov	r1, r2
    strb	r1, [r3, #10]
    ldr	r3, .LRfu
    mov	r2, #1
    mov	r1, r2
    strb	r1, [r3, #12]
    mov	r0, #1
    b	2f
1:
    mov	r3, #296
    add	r3, r3, #67108864
    ldrh	r2, [r3]
    mov	r1, r2, lsl #16
    mov	r3, r1, lsr #16
    str	r3, [fp, #-20]
    ldr	r2, [fp, #-20]
    and	r3, r2, #4
    ldrh	r2, [fp, #-14]
    mov	r1, r2, lsl #2
    cmp	r3, r1
    bne	3f
    b	4f
3:
    b	5b
4:
    mov	r0, #0
    b	2f
.LRfu:
    .word Rfu
2:
    ldmdb	fp, {fp, sp, lr}
    bx	lr
    .size Sio32WaitSIState, .-Sio32WaitSIState
");
#else
u16 Sio32WaitSIState(u32 State)
{
#if 1
    u32 x;

    for (x = 0; x != 0x9800 && Rfu.unk_12 != 1; x++) {
        if ((*(vu16 *)REG_SIOCNT & SIO_MULTI_SI) == (State << 2)) return 0;
    }

    Rfu.error = 3;
    Rfu.unk_07 = 1;
    return 1;
#else
    vu32 x;
    vu32 siocnt;
    vu16 state;
    u8 tmp;

    state = State;
    x = 0;

loop:
    x++;

    __asm__(
    "ldr r3, =Rfu;"
    "ldrb %0, [r3, #0x11];"  // Rfu.unk_12
    : "=r"((u8)tmp) : : "r0", "r1");

    if (tmp == 1 && x == 0x9800) {
        __asm__(
        "ldr r3, =Rfu;"
        "mov r2, #3;"
        "mov r1, r2;"
        "strb r1, [r3, #0xa];"  // Rfu.error
        : : : "r1", "r2", "r3");
        __asm__(
        "ldr r3, =Rfu;"
        "mov r2, #1;"
        "mov r1, r2;"
        "strb r1, [r3, #0xc];"  // Rfu.unk_07
        : : : "r1", "r2", "r3");
        return 1;
    }

    __asm__(
    "mov r3, #0x128;"
    "add r3, r3, #0x4000000;"
    "ldrh r2, [r3];"
    "mov r1, r2, lsl #0x10;"
    "mov %0, r1, lsr #0x10;"
    : "=r"(siocnt) : : "r1", "r2");

    if ((state << 2) == (siocnt & SIO_MULTI_SI)) return 0;
    goto loop;
#endif
}
#endif

__asm__("
Call_thumb:
    bx r0
");
