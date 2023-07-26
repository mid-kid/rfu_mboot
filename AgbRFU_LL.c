#include <Agb.h>
#include <stddef.h>

#include "GameInfo.h"
#include "RfuPeer.h"
#include "STWI_status.h"
#include "rfuLinkStatus.h"
#include "rfuStatic.h"

extern struct RfuPeer rfuSlotStatus_NI[4];
extern struct STWI_status STWI_status;
extern struct rfuLinkStatus rfuLinkStatus;
extern struct rfuStatic rfuStatic;
extern u8 RfuDataSendBuf[];
extern u8 STWI_buffer_recv[0x120];
extern void (*STWI_intr)(void);

extern u16  RfuMbootDLStart(u8 param_1,u8 param_2,u16 param_3,u16 *GameID,u32 param_5);
extern u16  STWI_send_CP_EndREQ(void);
extern u16  STWI_send_CP_StartREQ(u16 BeaconID);
extern u16  STWI_send_DataRxREQ(void);
extern u16  STWI_send_DataTxREQ(u8 *Srcp,u8 Size);
extern u16  STWI_send_GameConfigREQ(char *GameData,char *UserName);
extern u16  STWI_send_LinkStatusREQ(void);
extern u16  STWI_send_MS_ChangeREQ(void);
extern u16  STWI_send_ResetREQ(void);
extern u16  STWI_send_SP_EndREQ(void);
extern u16  STWI_send_SP_PollingREQ(void);
extern u16  STWI_send_SP_StartREQ(void);
extern u16  STWI_send_SystemConfigREQ(u16 param_1,u8 param_2,u8 param_3);
extern u16  STWI_send_SystemStatusREQ(void);
extern u32  rfu_clearAllSlot(void);
extern u32  rfu_constructSendLLFrame(void);
extern void RfuResetSub(u8 param_1);
extern void STWI_init_all(void);
extern void STWI_intr_vblank(void);
extern void Sio32IntrProcSet();
extern void rfu_REQ_disconnect(u8 Peer,u8 param_2);

extern struct rfuFixed {
	u8 *recv;
	u8 *send;
	u8 func[1];
} rfuFixed;

extern struct RfuEnc {
	u8 unk_01;
	u8 unk_02;
	u8 unk_03;
	u8 unk_04;
	u8 unk_05;
	u8 unk_06;
	u8 unk_07;
	u8 unk_08;
	u8 unk_09;
	u8 unk_10;
	u8 unk_11;
	u8 unk_12;
	u8 unk_13;
	u16 unk_15;
} llsf_struct[2];

static u16  rfu_STC_NI_constructLLSF(u8 Peer,u8 **Destp,struct RfuPeerSub *PeerSub);
static u16  rfu_STC_NI_initSlot_asRecvDataEntity(u8 Peer,struct RfuPeerSub *Sub);
static u16  rfu_STC_analyzeLLSF(u32 unused,u8 *Srcp,u16 Size);
static u32  rfu_STC_NI_initSlot_asRecvControllData(u8 Peer,struct RfuPeerSub *Sub);
static void rfu_STC_CHILD_analyzeRecvPacket(void);
static void rfu_STC_NI_receive_Receiver(u8 param_1,u8 *param_2,u8 *param_3);
static void rfu_STC_NI_receive_Sender(u8 param_1,u8 param_2,u8 *param_3,u8 *param_4);
static void rfu_STC_clearAPIVariables(void);
static void rfu_STC_fastCopy(u8 **Src,u8 **Dst,int Size);
static void rfu_STC_readParentCandidateList(void);

u32 rfu_REQ_RFUStatus(u8 *Recv)
{
	u16 ret;
	
	ret=STWI_send_SystemStatusREQ();
	if(ret==0)
		*Recv=rfuFixed.recv[7];
	else
		*Recv=-1;
	
	return ret;
}

void rfu_initializeAPI(void)
{
	u16 peer;
	u16 size;
	u16 *src;
	u16 *dst;
	
	STWI_init_all();
	rfu_STC_clearAPIVariables();
	rfuLinkStatus.timer=0;
	rfuFixed.recv=STWI_buffer_recv;
	
	for(peer=0;peer<4;peer++) {
		rfuSlotStatus_NI[peer].mbootDest=NULL;
		rfuSlotStatus_NI[peer].mbootSize=0;
	}
	
	src=(u16 *)((u32)rfu_STC_fastCopy & ~1);
	dst=(u16 *)rfuFixed.func;
	size=(u16 *)Sio32IntrProcSet-(u16 *)rfu_STC_fastCopy;
	while(--size!=(u16)-1)
		*dst++=*src++;
	
	rfuFixed.send=rfuFixed.func+1;
}

static void rfu_STC_clearAPIVariables(void)
{
	u8 save;
	
	CpuClear(0,&rfuStatic,16,16);
	save=rfuLinkStatus.timer;
	CpuClear(0,&rfuLinkStatus,0x5a*2,16);
	rfuStatic.timer=save;
	rfuLinkStatus.timer=save;
	rfuLinkStatus.mode=0xff;
	rfu_clearAllSlot();
}

u16 rfu_REQ_reset(void)
{
	u16 ret;
	
	ret=STWI_send_ResetREQ();
	if(ret==0)
		rfu_STC_clearAPIVariables();
	return ret;
}

u16 rfu_REQ_configGameData(u8 IsMultiboot,u16 GameID,char *GameName,char *UserName)
{
	u16 ret;
	u8 x;
	char *GameNameNew;
	char *UserNameCur;
	char *GameNameCur;
	char GameNameTmp[16];
	
	for(x=2;x<16;x++)
		GameNameTmp[x]=*GameName++;
	
	GameNameTmp[0]=GameID>>0;
	GameNameTmp[1]=GameID>>8;
	if(IsMultiboot)
		GameNameTmp[1]|=0x80;
	
	ret=STWI_send_GameConfigREQ(GameNameTmp,UserName);
	if(ret!=0)
		return ret;
	
	rfuLinkStatus.curGame.isMultiboot=IsMultiboot;
	rfuLinkStatus.curGame.gameID=GameID;
	
	GameNameCur=rfuLinkStatus.curGame.gameName;
	UserNameCur=rfuLinkStatus.curGame.userName;
	GameNameNew=GameNameTmp+2;
	
	for(x=0;x<0xe;x++) {
		*GameNameCur++=*GameNameNew++;
		
		if(x<8) {
			if(*UserName!='\0')
				*UserNameCur=*UserName++;
			else
				*UserNameCur='\0';
			UserNameCur++;
		}
	}
	
	return ret;
}

u16 rfu_REQ_configSystem(u16 param_1,u8 param_2,u8 param_3)
{
	return STWI_send_SystemConfigREQ(param_1 | 0x20,param_2,param_3);
}

u16 rfu_REQ_startSearchParent(void)
{
	return STWI_send_SP_StartREQ();
}

u16 rfu_REQ_pollSearchParent(void)
{
	u16 ret;
	
	ret=STWI_send_SP_PollingREQ();
	if(ret==0)
		rfu_STC_readParentCandidateList();
	return ret;
}

u16 rfu_REQ_endSearchParent(void)
{
	u16 ret;
	
	ret=STWI_send_SP_EndREQ();
	if(ret==0)
		rfu_STC_readParentCandidateList();
	return ret;
}

static void rfu_STC_readParentCandidateList(void)
{
	u8 x;
	u8 *data;
	u8 len;
	u8 peer;
	char *tmp;
	
	data=rfuFixed.recv;
	len=data[1];
	data+=4;
	
	rfuLinkStatus.gamesCount=0;
	for(peer=0;peer<4&&len!=0;peer++) {
		
		rfuLinkStatus.games[peer].beaconID=*((u16 *)data);
		data+=2;
		rfuLinkStatus.games[peer].playerNum=*((u8 *)data);
		data+=2;
		rfuLinkStatus.games[peer].gameID=*(u16 *)data & 0x7fff;
		if(*(u16 *)data & 0x8000)
			rfuLinkStatus.games[peer].isMultiboot=1;
		else
			rfuLinkStatus.games[peer].isMultiboot=0;
		data+=2;
		
		tmp=rfuLinkStatus.games[peer].gameName;
		for(x=0;x<14;x++)
			*tmp++=*data++;
		
		tmp=rfuLinkStatus.games[peer].userName;
		for(x=0;x<8;x++)
			*tmp++=*data++;
		
		len-=7;
		rfuLinkStatus.gamesCount++;
	}
}

u16 rfu_REQ_startConnectParent(u16 BeaconID)
{
	u16 ret;
	u16 x;
	
	for(x=0;x<4&&rfuLinkStatus.games[x].beaconID!=BeaconID;x++)
		;
	if(x==4)
		return 0x900;
	
	ret=STWI_send_CP_StartREQ(BeaconID);
	if(ret==0)
		rfuStatic.beaconID=BeaconID;
	return ret;
}

#ifndef NONMATCHING
__asm__ ("
.text
	.align	2
	.globl	rfu_REQ_pollConnectParent
	.type	 rfu_REQ_pollConnectParent,function
	.thumb_func
rfu_REQ_pollConnectParent:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	add	sp, sp, #-36
	add	r4, r0, #0
	mov	r8, r1
	mov	r7, #255
	strb	r7, [r4]
	strb	r7, [r1]
	bl	STWI_send_CP_PollingREQ
	lsl	r0, r0, #16
	lsr	r0, r0, #16
	mov	r9, r0
	cmp	r0, #0
	beq	.LCB19
	b	.LC17	@long jump
.LCB19:
	add	r6, sp, #32
	add	r0, r4, #0
	mov	r1, r8
	add	r2, r6, #0
	bl	rfu_getConnectParentStatus
	ldrb	r0, [r4]
	cmp	r0, #0
	bne	.LC17	@cond_branch
	mov	r0, #128
	lsl	r0, r0, #17
	mov	r1, r8
	ldrb	r1, [r1]
	lsl	r0, r0, r1
	lsr	r4, r0, #24
	ldr	r1, .LC15
	ldrb	r5, [r1, #2]
	add	r0, r4, #0
	and	r0, r0, r5
	add	r3, r1, #0
	cmp	r0, #0
	bne	.LC17	@long jump
.LC5:
	ldr	r1, .LC15+4
	ldrh	r0, [r1]
	mov	sl, r0
	mov	r2, #0
	mov	r0, r9
	strh	r0, [r1]
	add	r0, r4, #0
	orr	r0, r0, r5
	strb	r0, [r3, #2]
	ldrb	r0, [r3, #3]
	bic	r0, r0, r4
	strb	r0, [r3, #3]
	ldrh	r1, [r6]
	add	r0, r3, #0
	add	r0, r0, #148
	strh	r1, [r0]
	ldrb	r0, [r3, #1]
	add	r0, r0, #1
	strb	r0, [r3, #1]
	add	r1, r3, #0
	add	r1, r1, #10
	mov	r4, r8
	ldrb	r4, [r4]
	add	r1, r1, r4
	ldrb	r0, [r1]
	orr	r0, r0, r7
	strb	r0, [r1]
	strb	r2, [r3]
	mov	r6, #0
	ldr	r0, .LC15+8
	ldrh	r1, [r3, #20]
	add	r2, r0, #0
	ldrh	r0, [r2, #12]
	cmp	r1, r0
	bne	.LC8	@cond_branch
	ldrb	r0, [r3, #8]
	cmp	r0, #0
	bne	.LC9	@cond_branch
	add	r7, r3, #0
	add	r7, #20
	b	.LC7
.LC15:
	.word	rfuLinkStatus
	.word	67109384
	.word	rfuStatic
.LC9:
	mov	r7, sp
	lsl	r0, r6, #5
	ldr	r4, .LC20
	add	r0, r0, r4
	mov	r1, sp
	mov	r2, #16
	bl	CpuSet
	mov	r0, sp
	add	r0, #34
	mov	r5, #0
	strh	r5, [r0]
	ldr	r2, .LC20+4
	add	r1, r4, #0
	bl	CpuSet
	sub	r4, #20
	strb	r5, [r4, #8]
	b	.LC7
.LC21:
	.align	2
.LC20:
	.word	rfuLinkStatus+0x14
	.word	16777280
.LC8:
	add	r0, r6, #1
	lsl	r0, r0, #24
	lsr	r6, r0, #24
	cmp	r6, #3
	bhi	.LC14	@cond_branch
	lsl	r1, r6, #5
	add	r0, r1, r3
	ldrh	r0, [r0, #20]
	ldrh	r4, [r2, #12]
	cmp	r0, r4
	bne	.LC8	@cond_branch
	ldrb	r0, [r3, #8]
	cmp	r0, #0
	bne	.LC9	@cond_branch
	add	r0, r3, #0
	add	r0, #20
	add	r7, r1, r0
.LC7:
	cmp	r6, #3
	bhi	.LC14	@cond_branch
	mov	r0, r8
	ldrb	r1, [r0]
	lsl	r1, r1, #5
	ldr	r4, .LC22
	add	r1, r1, r4
	add	r0, r7, #0
	mov	r2, #16
	bl	CpuSet
	mov	r2, r8
	ldrb	r1, [r2]
	lsl	r0, r1, #5
	add	r4, r4, r0
	strb	r1, [r4, #2]
.LC14:
	ldr	r0, .LC22+4
	mov	r4, sl
	strh	r4, [r0]
.LC17:
	mov	r0, r9
	add	sp, sp, #36
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.LC23:
	.align	2
.LC22:
	.word	rfuLinkStatus+20
	.word	67109384
.LCfe1:
	.size	 rfu_REQ_pollConnectParent,.LCfe1-rfu_REQ_pollConnectParent
");
#else
u16 rfu_REQ_pollConnectParent(u8 *Busy,u8 *PlayerNum)
{
	u16 ime;
	u16 ret;
	u8 x;
	struct GameInfo *GameSrc;
	struct GameInfo GameTmp;
	u16 ID;
	u8 bit;
	
	GameSrc=&GameTmp;
	*Busy=-1;
	*PlayerNum=-1;
	
	ret=STWI_send_CP_PollingREQ();
	if(ret!=0)
		return ret;
	
	rfu_getConnectParentStatus(Busy,PlayerNum,&ID);
	if(*Busy!=0)
		return 0;
	
	bit=1<<*PlayerNum;
	if(rfuLinkStatus.peersConn & bit)
		return 0;
	
	ime=*(vu16 *)REG_IME;
	*(vu16 *)REG_IME=0;
	
	rfuLinkStatus.peersConn|=bit;
	rfuLinkStatus.peersSeen&=~bit;
	
	rfuLinkStatus.curGame.beaconID=ID;
	rfuLinkStatus.unk_01++;
	rfuLinkStatus.unk_0a[*PlayerNum]=-1;
	rfuLinkStatus.mode=0;
	
	for(x=0;x<4;x++) {
		if(rfuLinkStatus.games[x].beaconID==rfuStatic.beaconID) {
			if(rfuLinkStatus.gamesCount==0)
				GameSrc=rfuLinkStatus.games;
			else {
				GameSrc=&GameTmp;
				CpuCopy(rfuLinkStatus.games+x,&GameTmp,0x10*2,16);
				CpuArrayClear(0,rfuLinkStatus.games,16);
				rfuLinkStatus.gamesCount=0;
			}
			break;
		}
	}
	
	if(x<4) {
		CpuCopy(GameSrc,rfuLinkStatus.games+*PlayerNum,0x10*2,16);
		rfuLinkStatus.games[*PlayerNum].playerNum=*PlayerNum;
	}
	
	*(vu16 *)REG_IME=ime;
	return 0;
}
#endif

u16 rfu_REQ_endConnectParent(void)
{
	return STWI_send_CP_EndREQ();
}

void rfu_getConnectParentStatus(u8 *Busy,u8 *PlayerNum,u16 *ID)
{
	u8 *data;
	
	data=rfuFixed.recv;
	*ID=*(u16 *)(data+4);
	*PlayerNum=data[6];
	*Busy=data[7];
}

void rfu_setTimer(u8 val)
{
	struct rfuLinkStatus *ptr=&rfuLinkStatus;
	rfuStatic.timer=val;
	ptr->timer=val;
}

void rfu_syncVBlank(void)
{
	if(rfuLinkStatus.mode!=(u8)-1)
		if(rfuStatic.timer)
			rfuStatic.timer--;
	STWI_intr_vblank();
}

u32 rfu_REQBN_watchLink(u8 *PeersLost,u8 *Connected,u8 *PeersSeen)
{
	u8 *puVar6;
	
	u32 ret;
	u8 mode;
	u8 x;
	u8 bit;
	
	mode=0;
	ret=0;
	*PeersLost=0;
	*Connected=0;
	*PeersSeen=0;
	
	if(rfuLinkStatus.mode==(u8)-1)
		return 0;
	
	if(!rfuStatic.timer) {
		rfuStatic.timer=rfuLinkStatus.timer;
		mode=1;
	}
	
	if(rfuFixed.recv[0]==0x29) {
		*PeersLost=rfuFixed.recv[4];
		*Connected=rfuFixed.recv[5];
		if(*Connected==1)
			*PeersLost=rfuLinkStatus.peersConn;
		mode=2;
	}
	
	if(mode) {
		ret=STWI_send_LinkStatusREQ();
		if(ret==0) {
			puVar6=rfuFixed.recv+4;
			for(x=0;x<4;x++) {
				bit=1<<x;
				
				if(rfuLinkStatus.peersConn & bit&&!*puVar6&&mode==1) {
					*PeersLost|=bit;
					*Connected=1;
				}
				
				if(rfuLinkStatus.peersSeen & bit&&*puVar6&&rfuLinkStatus.mode==1) {
					*PeersSeen|=bit;
					rfuLinkStatus.peersConn|=bit;
					rfuLinkStatus.peersSeen&=~bit;
					rfuLinkStatus.unk_01++;
				}
				
				rfuLinkStatus.unk_0a[x]=*puVar6;
				puVar6++;
			}
		}
		
		for(x=0;x<4;x++) {
			if(rfuLinkStatus.peersConn & 1<<x&&*PeersLost & 1<<x)
				rfu_REQ_disconnect(x,FALSE);
		}
	}
	
	return ret;
}

void rfu_REQ_disconnect(u8 Peer,u8 Clear)
{
	u8 bit;
	
	bit=1<<Peer;
	if(rfuLinkStatus.peersConn & bit)
		if(rfuLinkStatus.unk_01)
			rfuLinkStatus.unk_01--;
	
	rfuLinkStatus.peersConn&=~bit;
	rfuLinkStatus.peersSeen|=bit;
	if(rfuLinkStatus.mode==0&&rfuLinkStatus.peersConn==0)
		rfuLinkStatus.mode=-1;
	
	if(Clear)
		CpuClear(0,rfuLinkStatus.games+Peer,sizeof(struct GameInfo),16);
}

static void rfu_STC_fastCopy(u8 **Src,u8 **Dst,int Size)
{
	u8 *src=*Src;
	u8 *dst=*Dst;
	
	while(Size--)
		*dst++=*src++;
	
	*Src=src;
	*Dst=dst;
}

void Sio32IntrProcSet(void (*Func)(void))
{
	STWI_intr=Func;
}

u16 rfu_REQ_changeMasterSlave(u8 param_1)
{
	u16 ret=0;
	
	if(param_1==0) {
		if(STWI_status.modeMaster==TRUE) {
			ret=STWI_send_MS_ChangeREQ();
			if(ret==0)
				rfuStatic.unk_02=0;
		}
	}
	else if(STWI_status.modeMaster==FALSE)
		rfuStatic.unk_02=param_1;
	
	return ret;
}

u16 rfu_REQ_changeMasterSlave_force(void)
{
	u16 ret=0;
	
	if(rfuStatic.unk_02==0)
		ret=STWI_send_MS_ChangeREQ();
	else
		rfuStatic.unk_02=0;
	
	return ret;
}

u8 rfu_getMasterSlave(void)
{
	return STWI_status.modeMaster;
}

u32 rfu_clearAllSlot(void)
{
	u16 x;
	
	for(x=0;x<4;x++) {
		CpuArrayClear(0,rfuSlotStatus_NI[x].sub,16);
		rfuLinkStatus.unk_10[x]=0x10;
	}
	
	rfuLinkStatus.unk_0f=0x57;
	rfuLinkStatus.unk_04=0;
	rfuLinkStatus.unk_05=0;
	rfuLinkStatus.unk_06=0;
	rfuStatic.unk_06=0;
	rfuStatic.unk_07=0;
	
	return 0;
}

static void rfu_STC_releaseFrame(u8 Peer,u8 Recv,struct RfuPeerSub *Sub)
{
	if(!Recv)
		rfuLinkStatus.unk_10[Peer]+=Sub->unk_21;
	rfuLinkStatus.unk_10[Peer]+=2;
}

u32 rfu_clearSlot(u8 Flags,u8 Peer)
{
	struct RfuPeerSub *sub;
	
	u32 ret;
	u16 x;
	u16 y;
	
	if(Peer>=4)
		return 0x600;
	
	ret=0x800;
	if(Flags & 0xc) {
		for(x=0;x<2;x++) {
			sub=NULL;
			if(x==0&&Flags & 0x4) {
				sub=&rfuSlotStatus_NI[Peer].sub[0];
				rfuLinkStatus.unk_04&=~(1<<Peer);
			}
			else if(x!=0&&Flags & 0x8) {
				sub=&rfuSlotStatus_NI[Peer].sub[1];
				rfuLinkStatus.unk_05&=~(1<<Peer);
			}
			
			if(sub!=NULL) {
				if(sub->unk_01[0] & 0x8000||
				   (sub->unk_01[0]==0x49&&sub->unk_04!=0x405)) {
					rfu_STC_releaseFrame(Peer,x,sub);
					for(y=0;y<4;y++)
						if(sub->unk_05 & 1<<y)
							sub->unk_01[1]=0;
				}
				
				CpuClear(0,sub,52,16);
			}
		}
		ret=0;
	}
	
	return ret;
}

u32 RfuMbootCfg(u8 param_1,u8 Peer,void *Dest,u32 Size)
{
	if(Peer>=4)
		return 0x600;
	
	if(param_1 & 0x20) {
		rfuSlotStatus_NI[Peer].mbootDest=Dest;
		rfuSlotStatus_NI[Peer].mbootSize=Size;
	}
	return 0;
}

u16 MbootDLStart1(u8 param_1,u16 param_2,u16 *GameID,u32 param_4)
{
	return RfuMbootDLStart(0x20,param_1,param_2,GameID,param_4);
}

u16 MbootDLStart2(u8 Peer,u16 param_2)
{
	return RfuMbootDLStart(0x40,1<<Peer,param_2,&rfuLinkStatus.curGame.gameID,0x1a);
}

#ifndef NONMATCHING
__asm__ ("
.text
	.align	2
	.globl	RfuMbootDLStart
	.type	 RfuMbootDLStart,function
	.thumb_func
RfuMbootDLStart:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	lsl	r0, r0, #24
	lsr	r6, r0, #24
	lsl	r1, r1, #24
	lsr	r3, r1, #24
	lsl	r2, r2, #16
	lsr	r2, r2, #16
	mov	r8, r2
	ldr	r0, .LB37
	ldrb	r1, [r0]
	mov	r9, r0
	cmp	r1, #255
	bne	.LB3	@cond_branch
	ldr	r0, .LB37+4
	b	.LB35
.LB38:
	.align	2
.LB37:
	.word	rfuLinkStatus
	.word	1282
.LB3:
	mov	r0, #15
	and	r0, r0, r3
	cmp	r0, #0
	bne	.LB5	@cond_branch
	ldr	r0, .LB39
	b	.LB35
.LB40:
	.align	2
.LB39:
	.word	1537
.LB5:
	mov	r1, r9
	ldrb	r0, [r1, #2]
	and	r0, r0, r3
	cmp	r0, r3
	beq	.LB7	@cond_branch
	ldr	r0, .LB41
	b	.LB35
.LB42:
	.align	2
.LB41:
	.word	1538
.LB7:
	mov	r2, r9
	ldrb	r0, [r2, #4]
	and	r0, r0, r3
	cmp	r0, #0
	beq	.LB9	@cond_branch
	ldr	r0, .LB43
	b	.LB35
.LB44:
	.align	2
.LB43:
	.word	1539
.LB9:
	mov	r1, #0
	mov	r0, #1
	and	r0, r0, r3
	mov	r5, r9
	add	r5, r5, #16
	ldr	r2, .LB45
	cmp	r0, #0
	bne	.LB12	@cond_branch
	mov	r4, #1
.LB13:
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bhi	.LB12	@cond_branch
	add	r0, r3, #0
	asr	r0, r0, r1
	and	r0, r0, r4
	cmp	r0, #0
	beq	.LB13	@cond_branch
.LB12:
	add	r7, r1, r5
	add	r0, r5, #0
	sub	r0, r0, #16
	ldrb	r0, [r0]
	lsl	r0, r0, #4
	add	r0, r0, r2
	ldrb	r5, [r0]
	ldrb	r0, [r7]
	cmp	r8, r0
	bhi	.LB18	@cond_branch
	cmp	r8, r5
	bhi	.LB17	@cond_branch
.LB18:
	mov	r0, #224
	lsl	r0, r0, #3
	b	.LB35
.LB46:
	.align	2
.LB45:
	.word	llsf_struct
.LB17:
	mov	r0, #32
	and	r0, r0, r6
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, #0
	bne	.LB21	@cond_branch
	cmp	r6, #64
	bne	.LB20	@cond_branch
.LB21:
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	ldr	r1, .LB47
	add	r0, r0, r1
	mov	ip, r0
	mov	r1, #0
	strh	r1, [r0, #24]
	mov	r1, ip
	add	r1, r1, #45
	mov	r2, ip
	str	r1, [r2, #4]
	mov	r0, #7
	str	r0, [r2, #20]
	mov	r0, ip
	add	r0, r0, #44
	strb	r3, [r0]
	strb	r3, [r2, #26]
	mov	r2, r8
	sub	r0, r2, r5
	mov	r2, ip
	strh	r0, [r2, #46]
	cmp	r4, #0
	beq	.LB22	@cond_branch
	mov	r0, #0
	b	.LB36
.LB48:
	.align	2
.LB47:
	.word	rfuSlotStatus_NI
.LB22:
	mov	r0, #1
.LB36:
	strb	r0, [r1]
	ldr	r0, [sp, #32]
	mov	r1, ip
	str	r0, [r1, #48]
	mov	r2, sl
	str	r2, [r1, #40]
	mov	r1, #0
	mov	r0, ip
	strb	r1, [r0, #31]
	add	r0, r0, #32
	strb	r1, [r0]
	mov	r4, ip
	add	r4, r4, #27
	mov	r6, #0
	mov	r2, ip
	add	r2, r2, #33
	mov	r5, #1
.LB27:
	add	r0, r4, r1
	strb	r6, [r0]
	add	r0, r2, r1
	strb	r5, [r0]
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bls	.LB27	@cond_branch
	mov	r1, #0
	mov	r6, #1
	ldr	r5, .LB49
	mov	r4, #0
.LB32:
	add	r0, r3, #0
	asr	r0, r0, r1
	and	r0, r0, r6
	cmp	r0, #0
	beq	.LB31	@cond_branch
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	add	r0, r0, r5
	strh	r4, [r0, #2]
.LB31:
	add	r0, r1, #1
	lsl	r0, r0, #24
	lsr	r1, r0, #24
	cmp	r1, #3
	bls	.LB32	@cond_branch
	mov	r2, r9
	ldrb	r1, [r2, #4]
	add	r0, r3, #0
	orr	r0, r0, r1
	strb	r0, [r2, #4]
	ldrb	r0, [r7]
	mov	r1, r8
	sub	r0, r0, r1
	strb	r0, [r7]
	ldr	r0, .LB49+4
	mov	r2, ip
	strh	r0, [r2]
.LB20:
	mov	r0, #0
.LB35:
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.LB50:
	.align	2
.LB49:
	.word	rfuSlotStatus_NI
	.word	32801
.LBfe1:
	.size	 RfuMbootDLStart,.LBfe1-RfuMbootDLStart
");
#else
u16 RfuMbootDLStart(u8 param_1,u8 param_2,u16 param_3,u16 *GameID,u32 param_5)
{
	u8 peer;
	u8 max;
	u8 *min;
	struct RfuPeerSub *sub;
	
	if(rfuLinkStatus.mode==(u8)-1)
		return 0x502;
	else if((param_2 & 0xf)==0)
		return 0x601;
	else if((rfuLinkStatus.peersConn & param_2)!=param_2)
		return 0x602;
	else if((rfuLinkStatus.unk_04 & param_2)!=0)
		return 0x603;
	else {
		for(peer=0;peer<4;peer++)
			if(param_2 & 1<<peer)
				break;
		min=&rfuLinkStatus.unk_10[peer];
		max=llsf_struct[rfuLinkStatus.mode].unk_01;
		if(param_3>*min||param_3<=max)
			return 0x700;
		else {
			if((param_1 & 0x20)!=0||param_1==0x40) {
				sub=&rfuSlotStatus_NI[peer].sub[0];
				sub->unk_04=0;
				sub->unk_02[0]=&sub->unk_20;
				sub->unk_03=7;
				sub->unk_19=param_2;
				sub->unk_05=param_2;
				sub->unk_21=param_3-max;
				if(param_1 & 0x20)
					sub->unk_20=0;
				else
					sub->unk_20=1;
				sub->unk_22=param_5;
				sub->unk_18=(u8 *)GameID;
				sub->unk_10=0;
				sub->unk_11=0;
				
				for(peer=0;peer<4;peer++) {
					sub->unk_06[peer]=0;
					sub->unk_12[peer]=1;
				}
				
				for(peer=0;peer<4;peer++) {
					if(param_2 & 1<<peer)
						rfuSlotStatus_NI[peer].sub[0].unk_01[1]=0;
				}
				rfuLinkStatus.unk_04|=param_2;
				*min-=param_3;
				sub->unk_01[0]=0x8021;
			}
			return 0;
		}
	}
}
#endif

u16 rfu_REQ_sendData(void)
{
	u16 res;
	u16 x;
	u32 size;
	struct RfuPeerSub *sub;
	
	res=0;
	if(rfuLinkStatus.mode==(u8)-1)
		return 0;
	
	rfuStatic.unk_12=0;
	size=rfu_constructSendLLFrame();
	if(rfuStatic.unk_12!=0)
		res=STWI_send_DataTxREQ(RfuDataSendBuf,size+4);
	
	if(res==0) {
		for(x=0;x<4;x++) {
			if(rfuSlotStatus_NI[x].sub[0].unk_01[0]!=0x8020)
				continue;
			
			sub=&rfuSlotStatus_NI[x].sub[0];
			
			rfu_STC_releaseFrame(x,FALSE,sub);
			
			rfuLinkStatus.unk_04&=~sub->unk_05;
			if(sub->unk_20==1)
				rfuLinkStatus.unk_07|=1<<x;
			sub->unk_01[0]=0x27;
		}
	}
	return res;
}

u32 rfu_constructSendLLFrame(void)
{
	u8 x;
	u8 *data;
	u32 peersize;
	u32 size;
	
	size=0;
	data=RfuDataSendBuf+4;
	
	for(x=0;x<4;x++) {
		peersize=0;
		
		if((rfuSlotStatus_NI[x].sub[0].unk_01[0] & 0x8000)!=0)
			peersize+=rfu_STC_NI_constructLLSF(x,&data,&rfuSlotStatus_NI[x].sub[0]);
		
		if((rfuSlotStatus_NI[x].sub[1].unk_01[0] & 0x8000)!=0)
			peersize+=rfu_STC_NI_constructLLSF(x,&data,&rfuSlotStatus_NI[x].sub[1]);
		
		if(peersize!=0) {
			if(rfuLinkStatus.mode==1)
				size+=peersize;
			else
				size|=peersize<<(x*5+8);
		}
	}
	
	if(size!=0) {
		while((u32)data & 3)
			*data++=0;
		*(u32 *)RfuDataSendBuf=size;
		
		if(rfuLinkStatus.mode==0)
			size=data-4-RfuDataSendBuf;
	}
	return size;
}

static u16 rfu_STC_NI_constructLLSF(u8 Peer,u8 **Destp,struct RfuPeerSub *PeerSub)
{
	u16 x;
	u16 size;
	u32 flags;
	u8 *enc;
	u8 *ptr;
	u8 *flags_ptr;
	u8 *temp_ptr;
	
	enc=(u8 *)&llsf_struct[rfuLinkStatus.mode];
	
	if(PeerSub->unk_01[0]==0x8022) {
		ptr=&PeerSub->unk_11;
		while(PeerSub->unk_02[*ptr]>=PeerSub->unk_18+PeerSub->unk_22)
			if(++(*ptr)==4)
				PeerSub->unk_11=0;
	}
	
	if(PeerSub->unk_01[0] & 0x40)
		size=0;
	else if(PeerSub->unk_01[0]==0x8022) {
		if(PeerSub->unk_02[PeerSub->unk_11]+PeerSub->unk_21>
		   PeerSub->unk_18+PeerSub->unk_22) {
			size=(PeerSub->unk_18+PeerSub->unk_22)-
				  PeerSub->unk_02[PeerSub->unk_11];
		}
		else
			size=PeerSub->unk_21;
	}
	else {
		if(PeerSub->unk_03>=PeerSub->unk_21)
			size=PeerSub->unk_21;
		else
			size=PeerSub->unk_03;
	}
	
	flags=
		(PeerSub->unk_01[0] & 0xf)<<enc[3]|
			PeerSub->unk_10<<enc[4]|
			PeerSub->unk_11<<enc[5]|
			PeerSub->unk_12[PeerSub->unk_11]<<enc[6]|
			size;
	
	if(rfuLinkStatus.mode==1)
		flags|=PeerSub->unk_05<<18;
	
	flags_ptr=(u8 *)&flags;
	for(x=0;x<enc[0];x++)
		*(*Destp)++=*flags_ptr++;
	
	if(size!=0) {
		temp_ptr=PeerSub->unk_02[PeerSub->unk_11];
		((void (*)())rfuFixed.send)(&temp_ptr,Destp,size);
	}
	
	if(PeerSub->unk_01[0]==0x8022) {
		ptr=&PeerSub->unk_11;
		if(++(*ptr)==4)
			PeerSub->unk_11=0;
	}
	
	rfuStatic.unk_12|=1<<Peer;
	
	return size+enc[0];
}

u32 rfu_REQ_recvData(void)
{
	u16 res;
	u8 x;
	struct RfuPeerSub *sub;
	
	if(rfuLinkStatus.mode==(u8)-1)
		return 0;
	
	rfuStatic.unk_10=rfuLinkStatus.unk_04 | rfuLinkStatus.unk_05 | rfuLinkStatus.unk_06;
	res=STWI_send_DataRxREQ();
	if(res==0&&rfuFixed.recv[1]!=0) {
		rfuStatic.unk_05=0;
		rfu_STC_CHILD_analyzeRecvPacket();
		
		for(x=0;x<4;x++) {
			if(rfuSlotStatus_NI[x].sub[1].unk_01[0]!=0x8043)
				continue;
			if(rfuStatic.unk_05 & 1<<x)
				continue;
			
			sub=&rfuSlotStatus_NI[x].sub[1];
			
			if(sub->unk_20==1)
				rfuLinkStatus.unk_07|=1<<x;
			rfu_STC_releaseFrame(x,TRUE,sub);
			rfuLinkStatus.unk_05&=~sub->unk_05;
			sub->unk_01[0]=0x47;
		}
	}
	return res;
}

static void rfu_STC_CHILD_analyzeRecvPacket(void)
{
	u16 done;
	u16 size;
	u8 *Srcp;
	
	size=*(u16 *)(rfuFixed.recv+4) & 0x7f;
	Srcp=rfuFixed.recv+8;
	
	if(size==0)
		rfuStatic.unk_05=0xf;
	
	while(size!=0) {
		done=rfu_STC_analyzeLLSF(0,Srcp,(u16)size);
		Srcp+=done;
		size-=done;
		if(size & 0x8000)
			break;
	}
}

static u16 rfu_STC_analyzeLLSF(u32 unused,u8 *Srcp,u16 Size)
{
	u16 x;
	struct RfuEnc *enc;
	u32 bits;
	struct {u8 _1; u8 _2; u8 _3; u8 _4; u8 _5; u8 _6; u16 _7;} fields;
	u16 ret;
	u8 mode;
	
	mode=~rfuLinkStatus.mode & 1;
	enc=&llsf_struct[mode];
	if(Size<enc->unk_01)
		return Size;
	
	bits=0;
	for(x=0;x<enc->unk_01;x++)
		bits|=*Srcp++<<x*8;
	
	fields._1=bits>>enc->unk_02 & enc->unk_08;
	fields._2=bits>>enc->unk_03 & enc->unk_09;
	fields._3=bits>>enc->unk_04 & enc->unk_10;
	fields._4=bits>>enc->unk_05 & enc->unk_11;
	fields._5=bits>>enc->unk_06 & enc->unk_12;
	fields._6=bits>>enc->unk_07 & enc->unk_13;
	fields._7=enc->unk_15 & bits;
	ret=fields._7+enc->unk_01;
	
	if(fields._1==0) {
		int temp=rfuLinkStatus.peersConn & fields._2;
		if(temp) {
			for(x=0;x<4;x++) {
				if(!(temp & 1<<x))
					continue;
				if((fields._4)==0)
					rfu_STC_NI_receive_Receiver(x,(u8 *)&fields,Srcp);
				else if(rfuLinkStatus.unk_04 & 1<<x)
					rfu_STC_NI_receive_Sender(x,x,(u8 *)&fields,Srcp);
			}
		}
	}
	
	return ret;
}

static void rfu_STC_NI_receive_Sender(u8 Peer,u8 param_2,u8 *param_3,u8 *param_4)
{
	u8 x;
	struct RfuPeerSub *sub;
	u16 save_unk_01;
	u8 save_unk_12;
	int diff;
	
	sub=&rfuSlotStatus_NI[Peer].sub[0];
	save_unk_01=sub->unk_01[0];
	save_unk_12=sub->unk_12[param_3[4]];
	
	if((param_3[2]==2&&sub->unk_01[0]==0x8022)||
	   (param_3[2]==1&&sub->unk_01[0]==0x8021)||
	   (param_3[2]==3&&sub->unk_01[0]==0x8023)) {
		if(sub->unk_12[param_3[4]]==param_3[5])
			sub->unk_06[param_3[4]]|=1<<param_2;
	}
	
	if((sub->unk_06[param_3[4]] & sub->unk_05)==sub->unk_05) {
		sub->unk_12[param_3[4]]=(sub->unk_12[param_3[4]]+1) & 3;
		sub->unk_06[param_3[4]]=0;
		
		if(sub->unk_01[0]==0x8021||
		   sub->unk_01[0]==0x8022) {
			if(sub->unk_01[0]==0x8021)
				sub->unk_02[param_3[4]]+=sub->unk_21;
			else
				sub->unk_02[param_3[4]]+=sub->unk_21*4;
			diff=sub->unk_03-sub->unk_21;
			sub->unk_03=diff;
			if(sub->unk_03==0||diff<0) {
				sub->unk_11=0;
				if(sub->unk_01[0]==0x8021) {
					for(x=0;x<4;x++) {
						sub->unk_12[x]=1;
						sub->unk_02[x]=&sub->unk_18[sub->unk_21*x];
					}
					sub->unk_03=sub->unk_22;
					sub->unk_01[0]=0x8022;
				}
				else {
					sub->unk_12[0]=0;
					sub->unk_03=0;
					sub->unk_01[0]=0x8023;
				}
			}
		}
		else if(sub->unk_01[0]==0x8023)
			sub->unk_01[0]=0x8020;
	}
	
	if(sub->unk_01[0]!=save_unk_01||
	   sub->unk_12[param_3[4]]!=save_unk_12||
	   sub->unk_06[param_3[4]] & 1<<param_2) {
		rfuStatic.unk_07|=1<<param_2;
		rfuSlotStatus_NI[param_2].sub[0].unk_01[1]=0;
	}
}

static void rfu_STC_NI_receive_Receiver(u8 Peer,u8 *param_2,u8 *param_3)
{
	u8 cont;
	struct RfuPeerSub *sub;
	u16 save_unk_01;
	u8 save_unk_12;
	u8 *ptr_unk_11;
	
	cont=FALSE;
	sub=&rfuSlotStatus_NI[Peer].sub[1];
	save_unk_01=sub->unk_01[0];
	save_unk_12=sub->unk_12[param_2[4]];
	
	if(sub->unk_01[0]==0x46||sub->unk_01[0]==0x47)
		return;
	
	if(param_2[2]==3) {
		rfuStatic.unk_05|=1<<Peer;
		ptr_unk_11=&sub->unk_11;
		if(sub->unk_01[0]==0x8042) {
			*ptr_unk_11=0;
			sub->unk_12[0]=0;
			sub->unk_01[0]=0x8043;
		}
	}
	else if(param_2[2]==2) {
		if(sub->unk_01[0]==0x8041&&sub->unk_03==0)
			sub->unk_01[0]=rfu_STC_NI_initSlot_asRecvDataEntity(Peer,sub);
		if(sub->unk_01[0]==0x8042)
			cont=TRUE;
	}
	else if(param_2[2]==1) {
		if(!(sub->unk_01[0] & 0x8000))
			sub->unk_01[0]=rfu_STC_NI_initSlot_asRecvControllData(Peer,sub);
		if(sub->unk_01[0]==0x8041)
			cont=TRUE;
	}
	
	if(cont&&param_2[5]==((sub->unk_12[param_2[4]]+1) & 3)) {
		((void (*)())rfuFixed.send)(&param_3,&sub->unk_02[param_2[4]],*(u16 *)&param_2[6]);
		if(sub->unk_01[0]==0x8042)
			sub->unk_02[param_2[4]]+=sub->unk_21*3;
		sub->unk_03-=*(u16 *)&param_2[6];
		sub->unk_12[param_2[4]]=param_2[5];
	}
	sub->unk_11=param_2[4];
	
	if(sub->unk_01[0]!=save_unk_01||
	   sub->unk_12[param_2[4]]!=save_unk_12||
	   sub->unk_12[param_2[4]]==param_2[5]) {
		rfuStatic.unk_06|=1<<Peer;
		sub->unk_01[1]=0;
	}
}

static u32 rfu_STC_NI_initSlot_asRecvControllData(u8 Peer,struct RfuPeerSub *Sub)
{
	u32 max;
	u8 *ptr;
	
	max=2;
	ptr=&rfuLinkStatus.unk_10[Peer];
	
	if(*ptr<max) {
		Sub->unk_04=0x405;
		return 0x49;
	}
	Sub->unk_04=0;
	
	*ptr-=max;
	Sub->unk_05=1<<Peer;
	Sub->unk_02[0]=&Sub->unk_20;
	Sub->unk_03=7;
	Sub->unk_10=1;
	Sub->unk_21=0;
	rfuLinkStatus.unk_05|=1<<Peer;
	return 0x8041;
}

static u16 rfu_STC_NI_initSlot_asRecvDataEntity(u8 Peer,struct RfuPeerSub *Sub)
{
	u8 x;
	
	if(Sub->unk_20==1)
		Sub->unk_02[0]=(u8 *)&rfuLinkStatus.games[Peer].gameID;
	else {
		if(rfuSlotStatus_NI[Peer].mbootDest==NULL) {
			rfuLinkStatus.unk_05&=~(1<<Peer);
			Sub->unk_04=0x401;
			return 0x49;
		}
		if(Sub->unk_22>rfuSlotStatus_NI[Peer].mbootSize) {
			rfuLinkStatus.unk_05&=~(1<<Peer);
			Sub->unk_04=0x402;
			return 0x49;
		}
		Sub->unk_02[0]=rfuSlotStatus_NI[Peer].mbootDest;
	}
	
	for(x=0;x<4;x++) {
		Sub->unk_12[x]=0;
		Sub->unk_02[x]=Sub->unk_02[0]+Sub->unk_21*x;
	}
	Sub->unk_03=Sub->unk_22;
	return 0x8042;
}

void RfuReset(void)
{
	u16 ime;
	
	ime=*(vu16 *)REG_IME;
	*(vu16 *)REG_IME=0;
	
	if(rfuLinkStatus.unk_04!=0)
		RfuResetSub(0);
	if(rfuLinkStatus.unk_05!=0)
		RfuResetSub(1);
	rfuStatic.unk_07=0;
	rfuStatic.unk_06=0;
	
	*(vu16 *)REG_IME=ime;
}

void RfuResetSub(u8 param_1)
{
	u8 x;
	u8 bit;
	u8 *puVar3;
	u8 *puVar4;
	
	if(param_1==0) {
		puVar4=&rfuLinkStatus.unk_04;
		puVar3=&rfuStatic.unk_07;
	}
	else {
		puVar4=&rfuLinkStatus.unk_05;
		puVar3=&rfuStatic.unk_06;
	}
	
	for(x=0;x<4;x++) {
		bit=1<<x;
		
		if((*puVar4 & bit)!=0&&
		   (*puVar3 & bit)==0&&
		   (param_1!=1||(rfuSlotStatus_NI[x].sub[1].unk_01[0]!=0x8043))) {
			if(param_1==0)
				rfuSlotStatus_NI[x].sub[0].unk_01[1]++;
			else
				rfuSlotStatus_NI[x].sub[1].unk_01[1]++;
		}
	}
}

