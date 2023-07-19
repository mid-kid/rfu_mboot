#include <Agb.h>

#include "Sio32.h"
extern struct Sio32 Sio32;
extern u32  Sio32IDMain(void);
extern void Sio32IDInit(void);
extern void Sio32IDIntr(void);
extern u8 u8_03005efc;
extern void (*STWI_intr)(void);

static const char Sio32ConnectionData[]={'N','I','N','T','E','N','D','O'};
static const char Sio32IDLib_Var[]="Sio32ID_011008";

u32 AgbRFU_checkID(void)
{
	u32 ret;
	u32 x;
	u16 ie;
	
	ie=*(vu16 *)REG_IE;
	u8_03005efc=1;
	
	STWI_intr=Sio32IDIntr;
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
	
	u8_03005efc=0;
	return ret;
}

void Sio32IDInit(void)
{
	*(vu16 *)REG_IME=0;
	*(vu16 *)REG_IE&=~(SIO_INTR_FLAG | TIMER3_INTR_FLAG);
	*(vu16 *)REG_IME=1;
	
	*(vu16 *)REG_RCNT=0;
	*(vu16 *)REG_SIOCNT=SIO_32BIT_MODE;
	*(vu16 *)REG_SIOCNT|=SIO_IF_ENABLE | SIO_ENABLE;
	CpuClear(0,&Sio32,sizeof(Sio32),32);
	*(vu16 *)REG_IF=SIO_INTR_FLAG | TIMER3_INTR_FLAG;
}

u32 Sio32IDMain(void)
{
	switch(Sio32.state) {
		case 0:
			Sio32.slave=TRUE;
			*(vu16 *)REG_SIOCNT|=SIO_SCK_IN;
			
			*(vu16 *)REG_IME=FALSE;
			*(vu16 *)REG_IE|=SIO_INTR_FLAG;
			*(vu16 *)REG_IME=TRUE;
			
			Sio32.state=1;
			*(vu8 *)REG_SIOCNT|=SIO_ENABLE;
			break;
			
		case 1:
			if(Sio32.deviceID==0) {
				if(Sio32.dataLo==0x8001)
					break;
				if(Sio32.slave)
					break;
				if(Sio32.handshakeStep!=0)
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
			
			Sio32.state=2;
			
		default:
			return Sio32.deviceID;
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
	.word	Sio32
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
	if(Sio32.slave!=TRUE)
		*(vu16 *)REG_SIOCNT|=0x80;
	
	DataHi=data<<((0+Sio32.slave)*16)>>16;
	DataLo=data<<((1-Sio32.slave)*16)>>16;
	
	if(Sio32.deviceID==0) {
		if(DataHi==Sio32.dataHi) {
			if(Sio32.handshakeStep<4) {
				if((u16) ~Sio32.dataLo==DataHi&&(u16) ~Sio32.dataHi==DataLo)
					Sio32.handshakeStep++;
			}
			else
				Sio32.deviceID=DataLo;
		}
		else
			Sio32.handshakeStep=0;
	}
	if(Sio32.handshakeStep<4)
		Sio32.dataLo=*(u16 *)(Sio32ConnectionData+Sio32.handshakeStep*2);
	else
		Sio32.dataLo=0x8001;  // Send device ID
	Sio32.dataHi=~DataLo;
	
	*(vu32 *)REG_SIODATA32=
		(Sio32.dataLo<<((1-Sio32.slave)*0x10))+
		(Sio32.dataHi<<((0+Sio32.slave)*0x10));
	
	if(Sio32.slave==TRUE&&Sio32.deviceID==0)
		*(vu16 *)REG_SIOCNT|=0x80;
}
#endif

