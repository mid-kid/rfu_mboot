#include <Agb.h>
#include <stddef.h>

#include "AgbRFU_LL.h"
#include "AgbRFU_LL_private.h"
#include "AgbRFU_STWI.h"
#include "AgbRFU_STWI_private.h"

#define LL_VER 0
#define LLSF_P_DLENGTH_SHIFT 0
#define LLSF_P_PHASE_SHIFT 9
#define LLSF_P_N_SHIFT 11
#define LLSF_P_ACK_SHIFT 13
#define LLSF_P_LCOM_SHIFT 14
#define LLSF_P_BMS_SHIFT 18
#define LLSF_P_LVER_SHIFT 22
#define LLSF_C_DLENGTH_SHIFT 0
#define LLSF_C_PHASE_SHIFT 5
#define LLSF_C_N_SHIFT 7
#define LLSF_C_ACK_SHIFT 9
#define LLSF_C_LCOM_SHIFT 10
#define LLSF_C_LVER_SHIFT 14
#define LLSF_P_LENGTH_MASK 0x0000007f
#define LLSF_P_PHASE_MASK 0x00000003
#define LLSF_P_N_MASK 0x00000003
#define LLSF_P_ACK_MASK 0x00000001
#define LLSF_P_LCOM_MASK 0x0000000f
#define LLSF_P_BMS_MASK 0x0000000f
#define LLSF_P_LVER_MASK 0x00000003
#define LLSF_C_LENGTH_MASK 0x0000001f
#define LLSF_C_PHASE_MASK 0x00000003
#define LLSF_C_N_MASK 0x00000003
#define LLSF_C_ACK_MASK 0x00000001
#define LLSF_C_LCOM_MASK 0x0000000f
#define LLSF_C_LVER_MASK 0x00000002
#define DATA_TYPE_USER_DATA 0
#define DATA_TYPE_GAME_NAME 1
#define TYPE_GNAME 0x40
#define RECV_END_COUNT 4
#define EEPROM_ADR 0x0000
#define EEPROM_RETRY_COUNT 16
#define REL_SEND 0
#define REL_RECV 1
#define EMBER_COUNT 16
#define NI_RECV_EXIST 0x80
#define WATCH_DOG_TIMER_COUNT 360
#define ID_KSRAC_INFO (ID_KEY_SET_READY_AND_CHANGE_REQ | 0x0100)
#define ERR_RECV_DATA_BLOCKED (ERR_DATA_RECV | 0x0009)
#define DEBUG_MODE_OFF (0x0000<<2)
#define DEBUG_MODE_ON (0x0001<<2)
#define RECOVERY_ACCEPT_STRENGTH_VALUE 10
#define LINK_TIMER_1SEC (0x0000<<3)
#define LINK_TIMER_2SEC (0x0001<<3)
#define LINK_TIMER_3SEC (0x0002<<3)
#define LINK_TIMER_4SEC (0x0003<<3)
#define SIO_MULTIPLAY_MODE 0x2000
#define RFU_WATCH_INTERVAL 4
#define CHILD_LS_FIXED_COUNT 240
#define CHILD_LS_DEFAULT_VALUE 0x10

extern u8 STWI_buffer_recv[0x120];
extern void (*STWI_callback_ID)(void);

typedef struct LinkLayerSubFrame_StructTag {
    u8 unk_01;  // header_size
    u8 unk_02;  // lver_s
    u8 unk_03;  // bmslot_s
    u8 unk_04;  // lcom_s
    u8 unk_05;  // ack_s
    u8 unk_06;  // phase_s
    u8 unk_07;  // n_s
    u8 unk_08;  // lver_mask
    u8 unk_09;  // bmslot_mask
    u8 unk_10;  // lcom_mask
    u8 unk_11;  // ack_mask
    u8 unk_12;  // phase_mask
    u8 unk_13;  // n_mask
    u16 unk_15;  // length_mask
}LLSF_STRUCT;

typedef struct LinkLayerSubFrame_HeaderTag {
    u8 lver;
    u8 bmslot;
    u8 lcom;
    u8 ack;
    u8 phase;
    u8 n;
    u16 length;
}LLSF_HEADER;

typedef struct UNameTag {
    // name
    // language_flag
    // dummy
    // sum
    // sum_rev
}U_NAME;

static const LLSF_STRUCT llsf_struct[2]={
    {0x02, 0x0e, 0x00, 0x0a, 0x09, 0x05, 0x07, 0x02, 0x00, 0x0f, 0x01, 0x03, 0x03, 0x1f},
    {0x03, 0x16, 0x12, 0x0e, 0x0d, 0x09, 0x0b, 0x03, 0x0f, 0x0f, 0x01, 0x03, 0x03, 0x7f}
};

static const char str_checkMbootLL[]="RFU-MBOOT";

RFU_SLOT_STATUS_NI rfuSlotStatus_NI[4];
RFU_LINK_STATUS rfuLinkStatus;
RFU_STATIC rfuStatic;
RFU_FIXED rfuFixed;

static u16  rfu_STC_NI_constructLLSF(u8 Peer,u8 **Destp,NI_COMM *Comm);
static u16  rfu_STC_NI_initSlot_asRecvDataEntity(u8 Peer,NI_COMM *Comm);
static u16  rfu_STC_analyzeLLSF(u32 unused,u8 *Srcp,u16 Size);
static u32  rfu_STC_NI_initSlot_asRecvControllData(u8 Peer,NI_COMM *Comm);
static void rfu_STC_CHILD_analyzeRecvPacket(void);
static void rfu_STC_NI_receive_Receiver(u8 param_1,u8 *param_2,u8 *param_3);
static void rfu_STC_NI_receive_Sender(u8 param_1,u8 param_2,u8 *param_3,u8 *param_4);
static void rfu_STC_clearAPIVariables(void);
static void rfu_STC_fastCopy(u8 **Src,u8 **Dst,int Size);
static void rfu_STC_incCommFailCounter(u8 param_1);
static void rfu_STC_readParentCandidateList(void);
static void rfu_STC_removeLinkData(u8 slot,u8 clear);

void rfu_setIDCallback(void (*callbackFuncp)(void));
u32  rfu_constructSendLLFrame(void);
u16  rfu_STC_setSendData_org(u8 param_1,u8 param_2,u16 param_3,const u16 *GameID,u32 param_5);

u16 rfu_getRFUStatus(u8 *rfuState)
{
	u16 ret;
	
	ret=STWI_send_SystemStatusREQ();
	if(ret==0)
		*rfuState=rfuFixed.dst[7];
	else
		*rfuState=-1;
	
	return ret;
}

void rfu_initializeAPI_NI(void)
{
	u16 peer;
	u16 size;
	u16 *src;
	u16 *dst;
	
	STWI_init_all();
	rfu_STC_clearAPIVariables();
	rfuLinkStatus.watchInterval=0;
	rfuFixed.dst=STWI_buffer_recv;
	
	for(peer=0;peer<4;peer++) {
		rfuSlotStatus_NI[peer].recvBuff=NULL;
		rfuSlotStatus_NI[peer].recvBuffSize=0;
	}
	
	src=(u16 *)((u32)rfu_STC_fastCopy & ~1);
	dst=(u16 *)rfuFixed.fastCopy_buff;
	size=(u16 *)rfu_setIDCallback-(u16 *)rfu_STC_fastCopy;
	while(--size!=(u16)-1)
		*dst++=*src++;
	
	rfuFixed.fastCopy_p=(void (*)())rfuFixed.fastCopy_buff+1;
}

static void rfu_STC_clearAPIVariables(void)
{
	u8 save;
	
	CpuClear(0,&rfuStatic,16,16);
	save=rfuLinkStatus.watchInterval;
	CpuClear(0,&rfuLinkStatus,0x5a*2,16);
	rfuStatic.now_watchInterval=save;
	rfuLinkStatus.watchInterval=save;
	rfuLinkStatus.parent_child=-1;
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

u16 rfu_REQ_configGameData(u8 mboot_flag,u16 serialNo,const u8 *GName,const u8 *UName)
{
	u16 ret;
	u8 x;
	char *GameNameNew;
	char *UserNameCur;
	char *GameNameCur;
	char GameNameTmp[16];
	
	for(x=2;x<16;x++)
		GameNameTmp[x]=*GName++;
	
	GameNameTmp[0]=serialNo>>0;
	GameNameTmp[1]=serialNo>>8;
	if(mboot_flag)
		GameNameTmp[1]|=0x80;
	
	ret=STWI_send_GameConfigREQ(GameNameTmp,UName);
	if(ret!=0)
		return ret;
	
	rfuLinkStatus.my.mboot_flag=mboot_flag;
	rfuLinkStatus.my.serialNo=serialNo;
	
	GameNameCur=rfuLinkStatus.my.gname;
	UserNameCur=rfuLinkStatus.my.uname;
	GameNameNew=GameNameTmp+2;
	
	for(x=0;x<0xe;x++) {
		*GameNameCur++=*GameNameNew++;
		
		if(x<8) {
			if(*UName!='\0')
				*UserNameCur=*UName++;
			else
				*UserNameCur='\0';
			UserNameCur++;
		}
	}
	
	return ret;
}

u16 rfu_REQ_configSystem(u16 availSlot_flag,u8 MaxMframe,u8 MC_Timer)
{
	return STWI_send_SystemConfigREQ(availSlot_flag | 0x20,MaxMframe,MC_Timer);
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
	
	data=rfuFixed.dst;
	len=data[1];
	data+=4;
	
	rfuLinkStatus.findParentCount=0;
	for(peer=0;peer<4&&len!=0;peer++) {
		
		rfuLinkStatus.partner[peer].id=*((u16 *)data);
		data+=2;
		rfuLinkStatus.partner[peer].slot=*((u8 *)data);
		data+=2;
		rfuLinkStatus.partner[peer].serialNo=*(u16 *)data & 0x7fff;
		if(*(u16 *)data & 0x8000)
			rfuLinkStatus.partner[peer].mboot_flag=1;
		else
			rfuLinkStatus.partner[peer].mboot_flag=0;
		data+=2;
		
		tmp=rfuLinkStatus.partner[peer].gname;
		for(x=0;x<14;x++)
			*tmp++=*data++;
		
		tmp=rfuLinkStatus.partner[peer].uname;
		for(x=0;x<8;x++)
			*tmp++=*data++;
		
		len-=7;
		rfuLinkStatus.findParentCount++;
	}
}

u16 rfu_REQ_startConnectParent(u16 pid)
{
	u16 ret;
	u16 x;
	
	for(x=0;x<4&&rfuLinkStatus.partner[x].id!=pid;x++)
		;
	if(x==4)
		return 0x900;
	
	ret=STWI_send_CP_StartREQ(pid);
	if(ret==0)
		rfuStatic.cid_bak=pid;
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
u16 rfu_REQ_pollConnectParent(u8 *status,u8 *connect_slotNo)
{
	u16 ime;
	u16 ret;
	u8 x;
	struct GameInfo *GameSrc;
	struct GameInfo GameTmp;
	u16 ID;
	u8 bit;
	
	GameSrc=&GameTmp;
	*status=-1;
	*connect_slotNo=-1;
	
	ret=STWI_send_CP_PollingREQ();
	if(ret!=0)
		return ret;
	
	rfu_getConnectParentStatus(status,connect_slotNo,&ID);
	if(*status!=0)
		return 0;
	
	bit=1<<*connect_slotNo;
	if(rfuLinkStatus.connectSlot_flag & bit)
		return 0;
	
	ime=*(vu16 *)REG_IME;
	*(vu16 *)REG_IME=0;
	
	rfuLinkStatus.connectSlot_flag|=bit;
	rfuLinkStatus.linkLossSlot_flag&=~bit;
	
	rfuLinkStatus.my.id=ID;
	rfuLinkStatus.connectCount++;
	rfuLinkStatus.strength[*connect_slotNo]=-1;
	rfuLinkStatus.parent_child=0;
	
	for(x=0;x<4;x++) {
		if(rfuLinkStatus.partner[x].id==rfuStatic.cid_bak) {
			if(rfuLinkStatus.findParentCount==0)
				GameSrc=rfuLinkStatus.partner;
			else {
				GameSrc=&GameTmp;
				CpuCopy(rfuLinkStatus.partner+x,&GameTmp,0x10*2,16);
				CpuArrayClear(0,rfuLinkStatus.partner,16);
				rfuLinkStatus.findParentCount=0;
			}
			break;
		}
	}
	
	if(x<4) {
		CpuCopy(GameSrc,rfuLinkStatus.partner+*connect_slotNo,0x10*2,16);
		rfuLinkStatus.partner[*connect_slotNo].slot=*connect_slotNo;
	}
	
	*(vu16 *)REG_IME=ime;
	return 0;
}
#endif

u16 rfu_REQ_endConnectParent(void)
{
	return STWI_send_CP_EndREQ();
}

void rfu_getConnectParentStatus(u8 *status,u8 *connect_slotNo,u16 *pid)
{
	u8 *data;
	
	data=rfuFixed.dst;
	*pid=*(u16 *)(data+4);
	*connect_slotNo=data[6];
	*status=data[7];
}

void rfu_setTimer(u8 val)
{
	RFU_LINK_STATUS *ptr=&rfuLinkStatus;
	rfuStatic.now_watchInterval=val;
	ptr->watchInterval=val;
}

void rfu_syncVBlank(void)
{
	if(rfuLinkStatus.parent_child!=(u8)-1)
		if(rfuStatic.now_watchInterval)
			rfuStatic.now_watchInterval--;
	STWI_intr_vblank();
}

u16 rfu_REQBN_watchLink(u8 *bm_linkLossSlot,u8 *bm_linkLossReason,u8 *parent_bm_linkRecoverySlot)
{
	u8 *puVar6;
	
	u16 ret;
	u8 mode;
	u8 x;
	u8 bit;
	
	mode=0;
	ret=0;
	*bm_linkLossSlot=0;
	*bm_linkLossReason=0;
	*parent_bm_linkRecoverySlot=0;
	
	if(rfuLinkStatus.parent_child==(u8)-1)
		return 0;
	
	if(!rfuStatic.now_watchInterval) {
		rfuStatic.now_watchInterval=rfuLinkStatus.watchInterval;
		mode=1;
	}
	
	if(rfuFixed.dst[0]==0x29) {
		*bm_linkLossSlot=rfuFixed.dst[4];
		*bm_linkLossReason=rfuFixed.dst[5];
		if(*bm_linkLossReason==1)
			*bm_linkLossSlot=rfuLinkStatus.connectSlot_flag;
		mode=2;
	}
	
	if(mode) {
		ret=STWI_send_LinkStatusREQ();
		if(ret==0) {
			puVar6=rfuFixed.dst+4;
			for(x=0;x<4;x++) {
				bit=1<<x;
				
				if(rfuLinkStatus.connectSlot_flag & bit&&!*puVar6&&mode==1) {
					*bm_linkLossSlot|=bit;
					*bm_linkLossReason=1;
				}
				
				if(rfuLinkStatus.linkLossSlot_flag & bit&&*puVar6&&rfuLinkStatus.parent_child==1) {
					*parent_bm_linkRecoverySlot|=bit;
					rfuLinkStatus.connectSlot_flag|=bit;
					rfuLinkStatus.linkLossSlot_flag&=~bit;
					rfuLinkStatus.connectCount++;
				}
				
				rfuLinkStatus.strength[x]=*puVar6;
				puVar6++;
			}
		}
		
		for(x=0;x<4;x++) {
			if(rfuLinkStatus.connectSlot_flag & 1<<x&&*bm_linkLossSlot & 1<<x)
				rfu_STC_removeLinkData(x,FALSE);
		}
	}
	
	return ret;
}

static void rfu_STC_removeLinkData(u8 slot,u8 clear)
{
	u8 bit;
	
	bit=1<<slot;
	if(rfuLinkStatus.connectSlot_flag & bit)
		if(rfuLinkStatus.connectCount)
			rfuLinkStatus.connectCount--;
	
	rfuLinkStatus.connectSlot_flag&=~bit;
	rfuLinkStatus.linkLossSlot_flag|=bit;
	if(rfuLinkStatus.parent_child==0&&rfuLinkStatus.connectSlot_flag==0)
		rfuLinkStatus.parent_child=-1;
	
	if(clear)
		CpuClear(0,rfuLinkStatus.partner+slot,sizeof(rfuTgtData),16);
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

void rfu_setIDCallback(void (*callbackFuncp)(void))
{
	STWI_callback_ID=callbackFuncp;
}

u16 rfu_REQ_changeMasterSlave_check(u8 param_1)
{
	u16 ret=0;
	
	if(param_1==0) {
		if(STWI_status.MS_mode==TRUE) {
			ret=STWI_send_MS_ChangeREQ();
			if(ret==0)
				rfuStatic.MS_mode=0;
		}
	}
	else if(STWI_status.MS_mode==FALSE)
		rfuStatic.MS_mode=param_1;
	
	return ret;
}

u16 rfu_REQ_changeMasterSlave(void)
{
	u16 ret=0;
	
	if(rfuStatic.MS_mode==0)
		ret=STWI_send_MS_ChangeREQ();
	else
		rfuStatic.MS_mode=0;
	
	return ret;
}

u8 rfu_getMasterSlave(void)
{
	return STWI_status.MS_mode;
}

u16 rfu_clearAllSlot(void)
{
	u16 x;
	
	for(x=0;x<4;x++) {
		CpuClear(0,&rfuSlotStatus_NI[x].send,sizeof(NI_COMM)*2,16);
		rfuLinkStatus.remainLLFrameSize_C[x]=0x10;
	}
	
	rfuLinkStatus.remainLLFrameSize_P=0x57;
	rfuLinkStatus.sendSlot_NI_flag=0;
	rfuLinkStatus.recvSlot_NI_flag=0;
	rfuLinkStatus.sendSlot_UNI_flag=0;
	rfuStatic.recv_renewal_flag=0;
	rfuStatic.send_renewal_flag=0;
	
	return 0;
}

static void rfu_STC_releaseFrame(u8 Peer,u8 Recv,NI_COMM *Comm)
{
	if(!Recv)
		rfuLinkStatus.remainLLFrameSize_C[Peer]+=Comm->payloadSize;
	rfuLinkStatus.remainLLFrameSize_C[Peer]+=2;
}

u16 rfu_clearSlot(u8 connType_flag,u8 slotStatus_Index)
{
	NI_COMM *comm;
	
	u16 ret;
	u16 x;
	u16 y;
	
	if(slotStatus_Index>=4)
		return 0x600;
	
	ret=0x800;
	if(connType_flag & 0xc) {
		for(x=0;x<2;x++) {
			comm=NULL;
			if(x==0&&connType_flag & 0x4) {
				comm=&rfuSlotStatus_NI[slotStatus_Index].send;
				rfuLinkStatus.sendSlot_NI_flag&=~(1<<slotStatus_Index);
			}
			else if(x!=0&&connType_flag & 0x8) {
				comm=&rfuSlotStatus_NI[slotStatus_Index].recv;
				rfuLinkStatus.recvSlot_NI_flag&=~(1<<slotStatus_Index);
			}
			
			if(comm) {
				if(comm->state & 0x8000||
						(comm->state==0x49&&comm->errorCode!=0x405)) {
					rfu_STC_releaseFrame(slotStatus_Index,x,comm);
					for(y=0;y<4;y++)
						if(comm->bmSlot & 1<<y)
							comm->failCounter=0;
				}
				
				CpuClear(0,comm,sizeof(NI_COMM),16);
			}
		}
		ret=0;
	}
	
	return ret;
}

u16 rfu_setRecvBuffer(u8 connType,u8 slotNo,void *buffer,u32 buffSize)
{
	if(slotNo>=4)
		return 0x600;
	
	if(connType & 0x20) {
		rfuSlotStatus_NI[slotNo].recvBuff=buffer;
		rfuSlotStatus_NI[slotNo].recvBuffSize=buffSize;
	}
	return 0;
}

u16 rfu_NI_setSendData(u8 bm_sendSlot,u16 subFrameSize,const void *src,u32 size)
{
	return rfu_STC_setSendData_org(0x20,bm_sendSlot,subFrameSize,src,size);
}

u16 rfu_NI_CHILD_setSendGameName(u8 slotNo,u16 subFrameSize)
{
	return rfu_STC_setSendData_org(0x40,1<<slotNo,subFrameSize,&rfuLinkStatus.my.serialNo,0x1a);
}

#ifndef NONMATCHING
//#if 0
__asm__ ("
.text
	.align	2
	.type	 rfu_STC_setSendData_org,function
	.thumb_func
rfu_STC_setSendData_org:
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
	.size	 rfu_STC_setSendData_org,.LBfe1-rfu_STC_setSendData_org
");
#else
u16 rfu_STC_setSendData_org(u8 param_1,u8 param_2,u16 param_3,u16 *GameID,u32 param_5)
{
	u8 peer;
	u8 max;
	u8 *min;
	NI_COMM *comm;
	
	if(rfuLinkStatus.parent_child==(u8)-1)
		return 0x502;
	else if((param_2 & 0xf)==0)
		return 0x601;
	else if((rfuLinkStatus.connectSlot_flag & param_2)!=param_2)
		return 0x602;
	else if((rfuLinkStatus.sendSlot_NI_flag & param_2)!=0)
		return 0x603;
	else {
		for(peer=0;peer<4;peer++)
			if(param_2 & 1<<peer)
				break;
		min=&rfuLinkStatus.remainLLFrameSize_C[peer];
		max=llsf_struct[rfuLinkStatus.parent_child].unk_01;
		if(param_3>*min||param_3<=max)
			return 0x700;
		else {
			if((param_1 & 0x20)!=0||param_1==0x40) {
				comm=&rfuSlotStatus_NI[peer].send;
				comm->errorCode=0;
				comm->nowp[0]=&comm->dataType;
				comm->remainSize=7;
				comm->bmSlot_org=param_2;
				comm->bmSlot=param_2;
				comm->payloadSize=param_3-max;
				if(param_1 & 0x20)
					comm->dataType=0;
				else
					comm->dataType=1;
				comm->dataSize=param_5;
				comm->src=(u8 *)GameID;
				comm->ack=0;
				comm->phase=0;
				
				for(peer=0;peer<4;peer++) {
					comm->recv_ack_flag[peer]=0;
					comm->n[peer]=1;
				}
				
				for(peer=0;peer<4;peer++) {
					if(param_2 & 1<<peer)
						rfuSlotStatus_NI[peer].send.failCounter=0;
				}
				rfuLinkStatus.sendSlot_NI_flag|=param_2;
				*min-=param_3;
				comm->state=0x8021;
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
	NI_COMM *comm;
	
	res=0;
	if(rfuLinkStatus.parent_child==(u8)-1)
		return 0;
	
	rfuStatic.llf_ready_flag=0;
	size=rfu_constructSendLLFrame();
	if(rfuStatic.llf_ready_flag!=0)
		res=STWI_send_DataTxREQ(rfuFixed.STWI_buf,size+4);
	
	if(res==0) {
		for(x=0;x<4;x++) {
			if(rfuSlotStatus_NI[x].send.state!=0x8020)
				continue;
			
			comm=&rfuSlotStatus_NI[x].send;
			
			rfu_STC_releaseFrame(x,FALSE,comm);
			
			rfuLinkStatus.sendSlot_NI_flag&=~comm->bmSlot;
			if(comm->dataType==1)
				rfuLinkStatus.getName_flag|=1<<x;
			comm->state=0x27;
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
	data=rfuFixed.STWI_buf+4;
	
	for(x=0;x<4;x++) {
		peersize=0;
		
		if((rfuSlotStatus_NI[x].send.state & 0x8000)!=0)
			peersize+=rfu_STC_NI_constructLLSF(x,&data,&rfuSlotStatus_NI[x].send);
		
		if((rfuSlotStatus_NI[x].recv.state & 0x8000)!=0)
			peersize+=rfu_STC_NI_constructLLSF(x,&data,&rfuSlotStatus_NI[x].recv);
		
		if(peersize!=0) {
			if(rfuLinkStatus.parent_child==1)
				size+=peersize;
			else
				size|=peersize<<(x*5+8);
		}
	}
	
	if(size!=0) {
		while((u32)data & 3)
			*data++=0;
		*(u32 *)rfuFixed.STWI_buf=size;
		
		if(rfuLinkStatus.parent_child==0)
			size=data-4-rfuFixed.STWI_buf;
	}
	return size;
}

static u16 rfu_STC_NI_constructLLSF(u8 Peer,u8 **Destp,NI_COMM *Comm)
{
	u16 x;
	u16 size;
	u32 flags;
	u8 *enc;
	u8 *ptr;
	u8 *flags_ptr;
	u8 *temp_ptr;
	
	enc=(u8 *)&llsf_struct[rfuLinkStatus.parent_child];
	
	if(Comm->state==0x8022) {
		ptr=&Comm->phase;
		while(Comm->nowp[*ptr]>=Comm->src+Comm->dataSize)
			if(++(*ptr)==4)
				Comm->phase=0;
	}
	
	if(Comm->state & 0x40)
		size=0;
	else if(Comm->state==0x8022) {
		if(Comm->nowp[Comm->phase]+Comm->payloadSize>
				Comm->src+Comm->dataSize) {
			size=Comm->src+Comm->dataSize-
				Comm->nowp[Comm->phase];
		}
		else
			size=Comm->payloadSize;
	}
	else {
		if(Comm->remainSize>=Comm->payloadSize)
			size=Comm->payloadSize;
		else
			size=Comm->remainSize;
	}
	
	flags=
		(Comm->state & 0xf)<<enc[3]|
		Comm->ack<<enc[4]|
		Comm->phase<<enc[5]|
		Comm->n[Comm->phase]<<enc[6]|
		size;
	
	if(rfuLinkStatus.parent_child==1)
		flags|=Comm->bmSlot<<18;
	
	flags_ptr=(u8 *)&flags;
	for(x=0;x<enc[0];x++)
		*(*Destp)++=*flags_ptr++;
	
	if(size!=0) {
		temp_ptr=Comm->nowp[Comm->phase];
		rfuFixed.fastCopy_p(&temp_ptr,Destp,size);
	}
	
	if(Comm->state==0x8022) {
		ptr=&Comm->phase;
		if(++(*ptr)==4)
			Comm->phase=0;
	}
	
	rfuStatic.llf_ready_flag|=1<<Peer;
	
	return size+enc[0];
}

u16 rfu_REQ_recvData(void)
{
	u16 res;
	u8 x;
	NI_COMM *comm;
	
	if(rfuLinkStatus.parent_child==(u8)-1)
		return 0;
	
	rfuStatic.commExist_flag=
		rfuLinkStatus.sendSlot_NI_flag|
		rfuLinkStatus.recvSlot_NI_flag|
		rfuLinkStatus.sendSlot_UNI_flag;
	
	res=STWI_send_DataRxREQ();
	if(res==0&&rfuFixed.dst[1]!=0) {
		rfuStatic.ni_end_recv_flag=0;
		rfu_STC_CHILD_analyzeRecvPacket();
		
		for(x=0;x<4;x++) {
			if(rfuSlotStatus_NI[x].recv.state!=0x8043)
				continue;
			if(rfuStatic.ni_end_recv_flag & 1<<x)
				continue;
			
			comm=&rfuSlotStatus_NI[x].recv;
			
			if(comm->dataType==1)
				rfuLinkStatus.getName_flag|=1<<x;
			rfu_STC_releaseFrame(x,TRUE,comm);
			rfuLinkStatus.recvSlot_NI_flag&=~comm->bmSlot;
			comm->state=0x47;
		}
	}
	return res;
}

static void rfu_STC_CHILD_analyzeRecvPacket(void)
{
	u16 done;
	u16 size;
	u8 *Srcp;
	
	size=*(u16 *)(rfuFixed.dst+4) & 0x7f;
	Srcp=rfuFixed.dst+8;
	
	if(size==0)
		rfuStatic.ni_end_recv_flag=0xf;
	
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
	const LLSF_STRUCT *enc;
	u32 bits;
	struct {u8 _1; u8 _2; u8 _3; u8 _4; u8 _5; u8 _6; u16 _7;} fields;
	u16 ret;
	u8 mode;
	
	mode=~rfuLinkStatus.parent_child & 1;
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
		int temp=rfuLinkStatus.connectSlot_flag & fields._2;
		if(temp) {
			for(x=0;x<4;x++) {
				if(!(temp & 1<<x))
					continue;
				if((fields._4)==0)
					rfu_STC_NI_receive_Receiver(x,(u8 *)&fields,Srcp);
				else if(rfuLinkStatus.sendSlot_NI_flag & 1<<x)
					rfu_STC_NI_receive_Sender(x,x,(u8 *)&fields,Srcp);
			}
		}
	}
	
	return ret;
}

static void rfu_STC_NI_receive_Sender(u8 Peer,u8 param_2,u8 *param_3,u8 *param_4)
{
	u8 x;
	NI_COMM *comm;
	u16 save_unk_01;
	u8 save_unk_12;
	int size;
	
	comm=&rfuSlotStatus_NI[Peer].send;
	save_unk_01=comm->state;
	save_unk_12=comm->n[param_3[4]];
	
	if((param_3[2]==2&&comm->state==0x8022)||
			(param_3[2]==1&&comm->state==0x8021)||
			(param_3[2]==3&&comm->state==0x8023)) {
		if(comm->n[param_3[4]]==param_3[5])
			comm->recv_ack_flag[param_3[4]]|=1<<param_2;
	}
	
	if((comm->recv_ack_flag[param_3[4]] & comm->bmSlot)==comm->bmSlot) {
		comm->n[param_3[4]]=(comm->n[param_3[4]]+1) & 3;
		comm->recv_ack_flag[param_3[4]]=0;
		
		if(comm->state==0x8021||
				comm->state==0x8022) {
			if(comm->state==0x8021)
				comm->nowp[param_3[4]]+=comm->payloadSize;
			else
				comm->nowp[param_3[4]]+=comm->payloadSize*4;
			size=comm->remainSize-comm->payloadSize;
			comm->remainSize=size;
			if(comm->remainSize==0||size<0) {
				comm->phase=0;
				if(comm->state==0x8021) {
					for(x=0;x<4;x++) {
						comm->n[x]=1;
						comm->nowp[x]=(u8 *)&comm->src[comm->payloadSize*x];
					}
					comm->remainSize=comm->dataSize;
					comm->state=0x8022;
				}
				else {
					comm->n[0]=0;
					comm->remainSize=0;
					comm->state=0x8023;
				}
			}
		}
		else if(comm->state==0x8023)
			comm->state=0x8020;
	}
	
	if(comm->state!=save_unk_01||
			comm->n[param_3[4]]!=save_unk_12||
			comm->recv_ack_flag[param_3[4]] & 1<<param_2) {
		rfuStatic.send_renewal_flag|=1<<param_2;
		rfuSlotStatus_NI[param_2].send.failCounter=0;
	}
}

static void rfu_STC_NI_receive_Receiver(u8 Peer,u8 *param_2,u8 *param_3)
{
	u8 cont;
	NI_COMM *comm;
	u16 save_unk_01;
	u8 save_unk_12;
	u8 *ptr_unk_11;
	
	cont=FALSE;
	comm=&rfuSlotStatus_NI[Peer].recv;
	save_unk_01=comm->state;
	save_unk_12=comm->n[param_2[4]];
	
	if(comm->state==0x46||comm->state==0x47)
		return;
	
	if(param_2[2]==3) {
		rfuStatic.ni_end_recv_flag|=1<<Peer;
		ptr_unk_11=&comm->phase;
		if(comm->state==0x8042) {
			*ptr_unk_11=0;
			comm->n[0]=0;
			comm->state=0x8043;
		}
	}
	else if(param_2[2]==2) {
		if(comm->state==0x8041&&comm->remainSize==0)
			comm->state=rfu_STC_NI_initSlot_asRecvDataEntity(Peer,comm);
		if(comm->state==0x8042)
			cont=TRUE;
	}
	else if(param_2[2]==1) {
		if(!(comm->state & 0x8000))
			comm->state=rfu_STC_NI_initSlot_asRecvControllData(Peer,comm);
		if(comm->state==0x8041)
			cont=TRUE;
	}
	
	if(cont&&param_2[5]==((comm->n[param_2[4]]+1) & 3)) {
		rfuFixed.fastCopy_p(&param_3,&comm->nowp[param_2[4]],*(u16 *)&param_2[6]);
		if(comm->state==0x8042)
			comm->nowp[param_2[4]]+=comm->payloadSize*3;
		comm->remainSize-=*(u16 *)&param_2[6];
		comm->n[param_2[4]]=param_2[5];
	}
	comm->phase=param_2[4];
	
	if(comm->state!=save_unk_01||
			comm->n[param_2[4]]!=save_unk_12||
			comm->n[param_2[4]]==param_2[5]) {
		rfuStatic.recv_renewal_flag|=1<<Peer;
		comm->failCounter=0;
	}
}

static u32 rfu_STC_NI_initSlot_asRecvControllData(u8 Peer,NI_COMM *Comm)
{
	u32 max;
	u8 *ptr;
	
	max=2;
	ptr=&rfuLinkStatus.remainLLFrameSize_C[Peer];
	
	if(*ptr<max) {
		Comm->errorCode=0x405;
		return 0x49;
	}
	Comm->errorCode=0;
	
	*ptr-=max;
	Comm->bmSlot=1<<Peer;
	Comm->nowp[0]=&Comm->dataType;
	Comm->remainSize=7;
	Comm->ack=1;
	Comm->payloadSize=0;
	rfuLinkStatus.recvSlot_NI_flag|=1<<Peer;
	return 0x8041;
}

static u16 rfu_STC_NI_initSlot_asRecvDataEntity(u8 Peer,NI_COMM *Comm)
{
	u8 x;
	
	if(Comm->dataType==1)
		Comm->nowp[0]=(u8 *)&rfuLinkStatus.partner[Peer].serialNo;
	else {
		if(rfuSlotStatus_NI[Peer].recvBuff==NULL) {
			rfuLinkStatus.recvSlot_NI_flag&=~(1<<Peer);
			Comm->errorCode=0x401;
			return 0x49;
		}
		if(Comm->dataSize>rfuSlotStatus_NI[Peer].recvBuffSize) {
			rfuLinkStatus.recvSlot_NI_flag&=~(1<<Peer);
			Comm->errorCode=0x402;
			return 0x49;
		}
		Comm->nowp[0]=rfuSlotStatus_NI[Peer].recvBuff;
	}
	
	for(x=0;x<4;x++) {
		Comm->n[x]=0;
		Comm->nowp[x]=Comm->nowp[0]+Comm->payloadSize*x;
	}
	Comm->remainSize=Comm->dataSize;
	return 0x8042;
}

void rfu_NI_checkCommFailCounter(void)
{
	u16 ime;
	
	ime=*(vu16 *)REG_IME;
	*(vu16 *)REG_IME=0;
	
	if(rfuLinkStatus.sendSlot_NI_flag)
		rfu_STC_incCommFailCounter(0);
	if(rfuLinkStatus.recvSlot_NI_flag)
		rfu_STC_incCommFailCounter(1);
	rfuStatic.send_renewal_flag=0;
	rfuStatic.recv_renewal_flag=0;
	
	*(vu16 *)REG_IME=ime;
}

static void rfu_STC_incCommFailCounter(u8 which)
{
	u8 x;
	u8 bit;
	u8 *puVar3;
	u8 *puVar4;
	
	if(which==0) {
		puVar4=&rfuLinkStatus.sendSlot_NI_flag;
		puVar3=&rfuStatic.send_renewal_flag;
	}
	else {
		puVar4=&rfuLinkStatus.recvSlot_NI_flag;
		puVar3=&rfuStatic.recv_renewal_flag;
	}
	
	for(x=0;x<4;x++) {
		bit=1<<x;
		
		if((*puVar4 & bit)!=0&&
				(*puVar3 & bit)==0&&
				(which!=1||(rfuSlotStatus_NI[x].recv.state!=0x8043))) {
			if(which==0)
				rfuSlotStatus_NI[x].send.failCounter++;
			else
				rfuSlotStatus_NI[x].recv.failCounter++;
		}
	}
}

