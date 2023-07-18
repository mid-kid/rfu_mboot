#include <Agb.h>

#include "STWI_status.h"
extern u16 STWI_poll_CommandEnd(void);
extern u16 STWI_check_Command(u32 Cmd, u8 VarSize);
extern u8 STWI_buffer_send[0x120];
extern u8 STWI_buffer_recv[0x120];
extern struct STWI_status STWI_status;

u32 STWI_init(void)
{
    *(vu16 *)REG_RCNT = 0x100;
    *(vu16 *)REG_SIOCNT = 0x5003;
    STWI_status.modeMaster = TRUE;
    STWI_status.state = 0;
    STWI_status.cmdHeader = 0;
    STWI_status.cmdSize = 0;
    STWI_status.field3_0x9 = 0;
    STWI_status.error = 0;
    STWI_status.modeMaster = TRUE;
    STWI_status.unk_07 = 0;
    STWI_status.unk_08 = 0;
    STWI_status.timer = -1;
    STWI_status.unk_11 = 0;
    return 0;
}

u32 STWI_init_all(void)
{
    STWI_init();
    STWI_status.unk_09 = 8;
    STWI_status.unk_12 = 1;
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_IE |= 0x81;
    *(vu16 *)REG_IME = 1;
    return 0;
}

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

u16 STWI_check_Command(u32 Cmd, u8 VarSize)
{
    u32 Head;

    // An error happened locally
    if (STWI_status.error != 0) return STWI_status.error;

    // An error has been received
    if (*(u32 *)STWI_buffer_recv == 0x996601ee) return 4;

    Head = *(u32 *)STWI_buffer_recv;
    if (VarSize == TRUE) Head = Head & 0xffff00ff;
    if (Head == Cmd) return 0;
    return 6;
}

u32 STWI_intr_vblank(void)
{
    if (STWI_status.modeMaster == TRUE) return 0;

    if (STWI_status.timer != (u8)-1) STWI_status.timer++;

    if (STWI_status.timer == 6) {
        STWI_status.state = 5;
        STWI_status.cmdHeader = 0;
        STWI_status.cmdSize = 0;
        STWI_status.field3_0x9 = 0;
        STWI_status.error = 0;
        STWI_status.unk_07 = 0;
        STWI_status.unk_08 = 0;
        *(vu32 *)REG_SIODATA32 = 0x80000000;
        *(vu16 *)REG_SIOCNT = 0;
        *(vu16 *)REG_SIOCNT = 0x5003;
        *(vu16 *)REG_SIOCNT = 0x5082;
        STWI_status.timer = -1;
        return 1;
    }
    return 0;
}

u16 STWI_send_ResetREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660010;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x99660090, FALSE);
    }
}

u16 STWI_send_LinkStatusREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660011;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x99660191, FALSE);
    }
}

u16 STWI_send_SystemStatusREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660013;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x99660193, FALSE);
    }
}

#ifndef NONMATCHING
__asm__("
.text
	.align	2
	.globl	STWI_send_GameConfigREQ
	.type	 STWI_send_GameConfigREQ,function
	.thumb_func
STWI_send_GameConfigREQ:
	push	{r4, r5, lr}
	add	r4, r0, #0
	add	r5, r1, #0
	bl	STWI_init
	ldr	r0, .LL22
	ldr	r1, .LL22+4
	stmia	r0!, {r1}
	add	r2, r0, #0
	ldrh	r0, [r4]
	strh	r0, [r2]
	add	r2, r2, #2
	add	r4, r4, #2
	mov	r1, #13
.LL6:
	ldrb	r0, [r4]
	strb	r0, [r2]
	add	r2, r2, #1
	add	r4, r4, #1
	sub	r1, r1, #1
	cmp	r1, #0
	bge	.LL6	@cond_branch
	ldr	r3, .LL22+8
	mov	r1, #0
	b	.LL8
.LL23:
	.align	2
.LL22:
	.word	STWI_buffer_send
	.word	-1721367018
	.word	STWI_status
.LL12:
	strb	r0, [r2]
	add	r2, r2, #1
	add	r5, r5, #1
	add	r1, r1, #1
	cmp	r1, #7
	bgt	.LL9	@cond_branch
.LL8:
	ldrb	r0, [r5]
	cmp	r0, #0
	bne	.LL12	@cond_branch
	cmp	r1, #7
	bgt	.LL9	@cond_branch
	mov	r0, #0
.LL16:
	strb	r0, [r2]
	add	r2, r2, #1
	add	r1, r1, #1
	cmp	r1, #7
	ble	.LL16	@cond_branch
.LL9:
	mov	r0, #6
	strb	r0, [r3, #8]
	bl	STWI_poll_CommandEnd
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r0, #1
	beq	.LL19	@cond_branch
	ldr	r0, .LL24
	mov	r1, #0
	bl	STWI_check_Command
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	b	.LL21
.LL25:
	.align	2
.LL24:
	.word	-1721368426
.LL19:
	mov	r0, #5
.LL21:
	pop	{r4, r5}
	pop	{r1}
	bx	r1
.LLfe1:
	.size	 STWI_send_GameConfigREQ,.LLfe1-STWI_send_GameConfigREQ
");
#else
u16 STWI_send_GameConfigREQ(char *GameData, char *UserName)
{
    u8 *dst;
    u8 *src;
    int x;

    STWI_init();

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
    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x99660096, FALSE);
    }
}
#endif

u16 STWI_send_SystemConfigREQ(u16 param_1, u8 param_2, u8 param_3)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660117;
    *(u8 *)(STWI_buffer_send + 4) = param_3;
    *(u8 *)(STWI_buffer_send + 5) = param_2;
    *(u16 *)(STWI_buffer_send + 6) = param_1;
    STWI_status.cmdSize = 1;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x99660097, FALSE);
    }
}

u16 STWI_send_SP_StartREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x9966001c;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x9966009c, FALSE);
    }
}

u16 STWI_send_SP_PollingREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x9966001d;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x9966009d, TRUE);
    }
}

u16 STWI_send_SP_EndREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x9966001e;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x9966009e, TRUE);
    }
}

u16 STWI_send_CP_StartREQ(u16 BeaconID)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x9966011f;
    *(u32 *)(STWI_buffer_send + 4) = BeaconID;
    STWI_status.cmdSize = 1;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x9966009f, FALSE);
    }
}

u16 STWI_send_CP_PollingREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660020;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996601a0, FALSE);
    }
}

u16 STWI_send_CP_EndREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660021;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996601a1, FALSE);
    }
}

u16 STWI_send_DataTxREQ(u8 *Srcp, u8 Size)
{
    u16 CmdSize;

    STWI_init();

    CmdSize = Size / 4;
    if (Size % 4) CmdSize++;
    STWI_status.cmdSize = CmdSize;
    CmdSize <<= 8;

    *(u32 *)(STWI_buffer_send + 0) = 0x99660024 + CmdSize;
    CpuSet(Srcp, STWI_buffer_send + 4, STWI_status.cmdSize | DMA_32BIT_BUS);

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996600a4, TRUE);
    }
}

u16 STWI_send_DataRxREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660026;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996600a6, TRUE);
    }
}

u16 STWI_send_MS_ChangeREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660027;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996600a7, FALSE);
    }
}

u16 STWI_send_DisconnectREQ(u8 param_1)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x99660130;
    *(u32 *)(STWI_buffer_send + 4) = param_1;
    STWI_status.cmdSize = 1;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996600b0, FALSE);
    }
}

u16 STWI_send_StopModeREQ(void)
{
    STWI_init();
    *(u32 *)(STWI_buffer_send + 0) = 0x9966003d;
    STWI_status.cmdSize = 0;

    if (STWI_poll_CommandEnd() == 1) {
        return 5;
    } else {
        return STWI_check_Command(0x996600bd, FALSE);
    }
}
