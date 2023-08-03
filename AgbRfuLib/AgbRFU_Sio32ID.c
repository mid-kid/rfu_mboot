#include <Agb.h>

#include "AgbRFU_Sio32ID.h"
#include "AgbRFU_STWI_private.h"

#define SIO32ID_SEND_WAIT_COUNT 600

static const char Sio32ConnectionData[]={'N','I','N','T','E','N','D','O'};
static const char Sio32IDLib_Var[]="Sio32ID_011008";
static Sio32IDArea S32id;

u32 unused;
u8 STWI_callback_ID;

#define STATIC
static void Sio32IDInit(void);
static u32  Sio32IDMain(void);
STATIC void Sio32IDIntr(void);

u32 AgbRFU_checkID(void)
{
	u32 ret;
	u32 x;
	u16 ie;
	
	ie=*(vu16 *)REG_IE;
	STWI_callback_ID=TRUE;
	STWI_callback_MSC=Sio32IDIntr;
	
	Sio32IDInit();
	for(x=0;x<0x3c;x++) {
		VBlankIntrWait();
		ret=Sio32IDMain();
		if(ret!=0)
			break;
	}
	
	*(vu16 *)REG_IME=0;
	*(vu16 *)REG_IE=ie;
	*(vu16 *)REG_IME=1;
	
	STWI_callback_ID=FALSE;
	return ret;
}

static void Sio32IDInit(void)
{
	*(vu16 *)REG_IME=0;
	*(vu16 *)REG_IE&=~(SIO_INTR_FLAG | TIMER3_INTR_FLAG);
	*(vu16 *)REG_IME=1;
	
	*(vu16 *)REG_RCNT=0;
	*(vu16 *)REG_SIOCNT=SIO_32BIT_MODE;
	*(vu16 *)REG_SIOCNT|=SIO_IF_ENABLE | SIO_ENABLE;
	CpuClear(0,&S32id,sizeof(S32id),32);
	*(vu16 *)REG_IF=SIO_INTR_FLAG | TIMER3_INTR_FLAG;
}

static u32 Sio32IDMain(void)
{
	switch(S32id.State) {
		case 0:
			S32id.Type=TRUE;
			*(vu16 *)REG_SIOCNT|=SIO_SCK_IN;
			
			*(vu16 *)REG_IME=FALSE;
			*(vu16 *)REG_IE|=SIO_INTR_FLAG;
			*(vu16 *)REG_IME=TRUE;
			
			S32id.State=1;
			*(vu8 *)REG_SIOCNT|=SIO_ENABLE;
			break;
			
		case 1:
			if(S32id.Connected==0) {
				if(S32id.SendBak==0x8001)
					break;
				if(S32id.Type)
					break;
				if(S32id.SendCheckCount!=0)
					break;
				
				*(vu16 *)REG_IME=FALSE;
				*(vu16 *)REG_IE&=~SIO_INTR_FLAG;
				*(vu16 *)REG_IME=TRUE;
				
				*(vu16 *)REG_SIOCNT=0;
				*(vu16 *)REG_SIOCNT=SIO_32BIT_MODE;
				*(vu16 *)REG_IF=SIO_INTR_FLAG;
				*(vu16 *)REG_SIOCNT|=SIO_IF_ENABLE | SIO_START;
				
				*(vu16 *)REG_IME=FALSE;
				*(vu16 *)REG_IE|=SIO_INTR_FLAG;
				*(vu16 *)REG_IME=TRUE;
				break;
			}
			
			S32id.State=2;
			
		default:
			return S32id.Connected;
	}
	
	return 0;
}

#ifndef NONMATCHING
__asm__ ("
.text
	.align	2
	.globl	Sio32IDIntr
	.type	 Sio32IDIntr,function
	.thumb_func
Sio32IDIntr:
	push	{r4, r5, lr}
	ldr	r0, .LA14
	ldr	r3, [r0]
	ldr	r0, .LA14+4
	ldrb	r1, [r0]
	add	r4, r0, #0
	cmp	r1, #1
	beq	.LA3	@cond_branch
	ldr	r0, .LA14+8
	ldrh	r1, [r0]
	mov	r2, #128
	orr	r1, r1, r2
	strh	r1, [r0]
.LA3:
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
	bne	.LA4	@cond_branch
	ldrh	r2, [r4, #6]
	cmp	r1, r2
	bne	.LA5	@cond_branch
	ldrh	r5, [r4, #2]
	cmp	r5, #3
	bls	.LA6	@cond_branch
	strh	r3, [r4, #10]
	b	.LA4
.LA15:
	.align	2
.LA14:
	.word	67109152
	.word	S32id
	.word	67109160
.LA6:
	ldrh	r0, [r4, #4]
	mvn	r0, r0
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r1, r0
	bne	.LA4	@cond_branch
	mvn	r0, r2
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	cmp	r3, r0
	bne	.LA4	@cond_branch
	add	r0, r5, #1
.LA5:
	strh	r0, [r4, #2]
.LA4:
	ldrh	r0, [r4, #2]
	cmp	r0, #3
	bhi	.LA10	@cond_branch
	lsl	r0, r0, #1
	ldr	r1, .LA16
	add	r0, r0, r1
	ldrh	r0, [r0]
	b	.LA13
.LA17:
	.align	2
.LA16:
	.word	Sio32ConnectionData
.LA10:
	ldr	r0, .LA18
.LA13:
	strh	r0, [r4, #4]
	mvn	r0, r3
	strh	r0, [r4, #6]
	ldr	r3, .LA18+4
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
	bne	.LA12	@cond_branch
	ldrh	r0, [r4, #10]
	cmp	r0, #0
	bne	.LA12	@cond_branch
	ldr	r0, .LA18+8
	ldrh	r1, [r0]
	mov	r2, #128
	orr	r1, r1, r2
	strh	r1, [r0]
.LA12:
	pop	{r4, r5}
	pop	{r0}
	bx	r0
.LA19:
	.align	2
.LA18:
	.word	32769
	.word	67109152
	.word	67109160
.LAfe1:
	.size	 Sio32IDIntr,.LAfe1-Sio32IDIntr
");
#else
void Sio32IDIntr(void)
{
	u32 data;
	u32 DataHi;
	u32 DataLo;
	
	data=*(vu32 *)REG_SIODATA32;
	if(S32id.Type!=TRUE)
		*(vu16 *)REG_SIOCNT|=0x80;
	
	DataHi=data<<((0+S32id.Type)*16)>>16;
	DataLo=data<<((1-S32id.Type)*16)>>16;
	
	if(S32id.Connected==0) {
		if(DataHi==S32id.RecvBak) {
			if(S32id.SendCheckCount<4) {
				if((u16) ~S32id.SendBak==DataHi&&(u16) ~S32id.RecvBak==DataLo)
					S32id.SendCheckCount++;
			}
			else
				S32id.Connected=DataLo;
		}
		else
			S32id.SendCheckCount=0;
	}
	if(S32id.SendCheckCount<4)
		S32id.SendBak=*(u16 *)(Sio32ConnectionData+S32id.SendCheckCount*2);
	else
		S32id.SendBak=0x8001; // Send device ID
	S32id.RecvBak=~DataLo;
	
	*(vu32 *)REG_SIODATA32=
		(S32id.SendBak<<((1-S32id.Type)*0x10))+
		(S32id.RecvBak<<((0+S32id.Type)*0x10));
	
	if(S32id.Type==TRUE&&S32id.Connected==0)
		*(vu16 *)REG_SIOCNT|=0x80;
}
#endif

