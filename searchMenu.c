#include "rfuDefine.h"

#include <AgbDefine.h>
#include <AgbMemoryMap.h>
#include <AgbMacro.h>
#include <AgbSystemCall.h>
#include <AgbRFU_LL_private.h>
#include <AgbRFU_STWI.h>
#include "myFunc.h"
#include "data.h"
#include "sound.h"

extern u8 _binary_char_search_tmap_LZ_bin_start[];

enum {
	// Correspond to entries in SearchProcTable
	STATE_RESET,
	STATE_CONFIG_SYSTEM,
	STATE_CONFIG_GAME,
	STATE_SP_START,
	STATE_SP_POLL,
	STATE_SP_END,
	STATE_CP_START,
	STATE_CP_POLL,
	STATE_CP_END,
	STATE_CHANGE_CLOCK_SLAVE,
	STATE_INIT,
	STATE_RETURN_TITLE,
	STATE_LINK_STATUS,
	STATE_SYSTEM_STATUS,
	
	// Standalone states
	STATE_WAIT_SP = 0x84,
	STATE_MBOOT_START = 0x90,
	STATE_MBOOT_POLL = 0x91,
	STATE_WAIT = 0x92,
	STATE_DOWNLOAD_START = 0xa0,
	STATE_WAIT_DOWNLOAD_END = 0xa1,
	STATE_CONNECTION_ERROR = 0xc1,
	STATE_FATAL_ERROR = 0xc2,
	STATE_DOWNLOAD_FAILED = 0xc5,
	STATE_DOWNLOAD_SUCCESS = 0xc6,
	STATE_EXEC = 0xc7
};

__attribute__((nocommon)) u8 SearchMenuEnd;
__attribute__((nocommon)) u8 SearchMenuCursor;
__attribute__((nocommon)) u8 MbootPeer;
__attribute__((nocommon)) u8 SearchMenuErrorMsg;
__attribute__((nocommon)) u16 MbootBeaconID;
__attribute__((nocommon)) u16 SearchMenuTimer;
__attribute__((nocommon)) u8 blink_counter;
__attribute__((nocommon)) u32 DAT_0300569c;
__attribute__((nocommon)) rfuTgtData GameList[4];
__attribute__((nocommon)) u8 GameListBits;
__attribute__((nocommon)) u8 GameListBitsNew;

u8 SearchMenuErrorTimer;
u8 MenuBusy;
u8 Lang;
const MSGDATA *MenuMsg;
u8 my_state;
u8 MainMenuFadeOut;
char GameName[14];

#define STATIC
static u16  REQ_changeMasterSlave(void);
static u16  REQ_configGameData(void);
static u16  REQ_configSystem(void);
static u16  REQ_pollConnectParent(void);
static u16  REQ_startConnectParent(void);
static u16  my_softReset_and_checkID(void);
static u8   my_strcmp(const char *str1,const char *str2);
static void my_changeClockMaster(void);
STATIC void REQ_callback_mboot(void);
static void SEQ_search_dl(void);
static void SEQ_search_mboot(void);
static void my_drawListTitle(u16 Pos,u8 Len,u16 CharNo);

static u16(*const SearchProcTable[])(void)={
	rfu_REQ_reset,
	REQ_configSystem,
	REQ_configGameData,
	rfu_REQ_startSearchParent,
	rfu_REQ_pollSearchParent,
	rfu_REQ_endSearchParent,
	REQ_startConnectParent,
	REQ_pollConnectParent,
	rfu_REQ_endConnectParent,
	REQ_changeMasterSlave,
	my_softReset_and_checkID,
	STWI_send_StopModeREQ,
	STWI_send_LinkStatusREQ,
	STWI_send_SystemStatusREQ
};

void SEQ_search_init(void)
{
	u8 x;
	
	mf_winFade(0);
	
	mf_drawBg2_search(0x18);
	*(vu16 *)REG_BG2VOFS=-52;
	*(vu16 *)REG_BG2HOFS=-24;
	DmaClear(3,0,Bg0Bak,sizeof(Bg0Bak),16);
	LZ77UnCompVram(_binary_char_search_tmap_LZ_bin_start,(u8 *)VRAM+0x800);
	VBlankIntrWait();
	
	*(vu16 *)REG_DISPCNT|=DISP_BG1_ON;
	mf_winFade(1);
	*(vu16 *)REG_DISPCNT|=DISP_BG2_ON;
	
	if(Lang==0) {
		my_drawListTitle(0xa6,8,0x151);
		my_drawListTitle(0xb2,8,0x159);
	}
	else {
		my_drawListTitle(0xa7,6,0x161);
		my_drawListTitle(0xb3,7,0x167);
	}
	
	for(x=0;x<4;x++)
		GameList[x].id=0;
	rfu_clearAllSlot();
	SearchMenuEnd=FALSE;
	SearchMenuCursor=0;
	SearchMenuErrorTimer=0;
	SearchMenuErrorMsg=-1;
	my_state=STATE_INIT;
	menu_initGameName();
}

static void my_drawListTitle(u16 Pos,u8 Len,u16 CharNo)
{
	u16 *bg;
	
	bg=Bg0Bak+Pos;
	if(CharNo==0x161)
		bg[-31]=0x115;
	else if(CharNo==0x167)
		bg[-30]=0x13d;
	
	while(Len--)
		*bg++=CharNo++;
}

void SEQ_search(void)
{
	u16 procRes=0;
	
	if(rfuLinkStatus.parent_child==0) {
		rfu_NI_checkCommFailCounter();
		
		if(my_state!=STATE_DOWNLOAD_SUCCESS) {
			*(vu16 *)REG_IME=0;
			SearchMenuErrorTimer++;
			if(SearchMenuErrorTimer>4*60) {
				SearchMenuErrorTimer=0;
				my_state=STATE_INIT;
				SearchMenuErrorMsg=4;  // ERROR OCCURRED!
			}
			*(vu16 *)REG_IME=1;
		}
	}
	
	if(my_state==STATE_WAIT_SP||my_state==STATE_WAIT) {
		mf_rapidKey();
		
		// Move the cursor
		if(key.Trg & (U_KEY | D_KEY)) {
			if(key.Trg & U_KEY) {
				SearchMenuCursor--;
				if(SearchMenuCursor & 0x80)
					SearchMenuCursor=3;
			}
			else {
				SearchMenuCursor++;
				if(SearchMenuCursor>3)
					SearchMenuCursor=0;
			}
			*(vu16 *)REG_BG2VOFS=~(SearchMenuCursor*16+51);
			snd_play(0);
		}
		
		// Select game
		if(key.Trg & A_BUTTON&&GameList[SearchMenuCursor].id) {
			menu_blinkGame(FALSE);
			MbootBeaconID=GameList[SearchMenuCursor].id;
			if(my_state==STATE_WAIT_SP)
				my_state=STATE_SP_END;
			else
				my_state=STATE_CP_START;
			snd_play(2);
		}
	}
	
	if(key.Trg & B_BUTTON) {
		if(my_state==STATE_DOWNLOAD_START) {
			my_changeClockMaster();
			my_state=STATE_RESET;
			menu_initGameList();
			snd_play(3);
		}
		else if(!SearchMenuEnd&&!MenuBusy&&
				SearchMenuErrorMsg==(u8)-1&&
				my_state!=STATE_INIT&&
				my_state!=STATE_MBOOT_POLL) {
			my_changeClockMaster();
			my_state=STATE_RESET;
			SearchMenuEnd=TRUE;
			snd_play(3);
		}
	}
	
	if(!(my_state & 0x80))
		procRes=SearchProcTable[my_state]();
	
	switch(my_state) {
		case STATE_INIT:
			// Resets and boots the adapter
			if(procRes==0)
				my_state=STATE_RESET;
			break;
			
		case STATE_RESET:
			// Restart from scratch
			if(procRes==0) {
				SearchMenuErrorMsg=-1;
				if(SearchMenuEnd!=FALSE)
					my_state=STATE_RETURN_TITLE;
				else {
					my_state=STATE_CONFIG_SYSTEM;
					mf_clearRect(0x6b,1,8);
				}
			}
			break;
			
		case STATE_CONFIG_SYSTEM:
			// Configure the radio
			if(procRes==0) {
				my_state=STATE_SP_START;
				menu_initBlinkCounter();
				GameListBits=0;
			}
			break;
			
		case STATE_SP_START:
			// Search a parent
			if(procRes==0) {
				my_state=STATE_WAIT_SP;
				MbootBeaconID=0;
				SearchMenuTimer=1*60;
			}
			break;
			
		case STATE_WAIT_SP:
			// Wait one second for parent search to finish
			if(GameListBits) {
				menu_drawMessage(9,0);  // SELECT A GAME
			}
			else {
				menu_blinkMessage(8,0x40);  // NOW SEARCHING...
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				my_state=STATE_SP_END;
			break;
			
		case STATE_SP_END:
			// Get discovery results
			if(procRes==0) {
				if(SearchMenuTimer<60-8)
					GameListBits=menu_drawGameList();
				
				if(MbootBeaconID)
					my_state=STATE_CP_START;
				else {
					my_state=STATE_WAIT;
					SearchMenuTimer=5*60;
					blink_counter=0;
				}
			}
			break;
			
		case STATE_WAIT:
			// Allow the player to select a game
			// Restart discovery after 5 seconds
			if(GameListBits!=0) {
				menu_drawMessage(9,0);  // SELECT A GAME
				if(SearchMenuTimer>3*60)
					menu_blinkGame(TRUE);
				else
					menu_blinkGame(FALSE);
			}
			else {
				menu_blinkMessage(8,0x40);  // NOW SEARCHING...
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				my_state=STATE_SP_START;
			break;
			
		case STATE_CP_START:
			// Start connecting to a parent
			if(procRes==0) {
				mf_clearRect(0x200,2,0x20);
				SearchMenuTimer=2*60;
				my_state=STATE_CP_POLL;
			}
			else if(procRes==0x900) {
				procRes=0;
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				my_state=STATE_CONNECTION_ERROR;
			}
			break;
			
		case STATE_CP_POLL:
			// Wait a maximum of two seconds for the connection to complete
			if(procRes==0) {
				my_state=STATE_CP_END;
				break;
			}
			
			SearchMenuTimer--;
			if(procRes & 0x8000) {
				if((procRes & 0xff)==2)
					SearchMenuTimer=0;
				procRes=0;
			}
			if(SearchMenuTimer==0)
				my_state=STATE_CP_END;
			break;
			
		case STATE_CP_END:
			// Finalize connection
			if(procRes==0) {
				if(SearchMenuTimer>0&&rfuFixed.dst[7]==0) {
					menu_clearGameList();
					rfu_setMSCCallback(REQ_callback_mboot);
					rfu_setRecvBuffer(0x20,MbootPeer,(u8 *)EX_WRAM,EX_WRAM_SIZE);
					my_state=STATE_CONFIG_GAME;
				}
				else {
					SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
					my_state=STATE_CONNECTION_ERROR;
				}
			}
			break;
			
		case STATE_CONFIG_GAME:
			// Set the timer for the next step
			if(procRes==0) {
				SearchMenuTimer=0.5*60;
				my_state=STATE_LINK_STATUS;
			}
			break;
			
		case STATE_LINK_STATUS:
			// Check if parent is still connected
			if(procRes==0) {
				if(rfuFixed.dst[4+MbootPeer]) {
					my_state=STATE_CHANGE_CLOCK_SLAVE;
					mf_drawString(0x6b,2,rfuLinkStatus.my.uname);
					break;
				}
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0) {
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				my_state=STATE_CONNECTION_ERROR;
			}
			break;
			
		case STATE_CHANGE_CLOCK_SLAVE:
			// Switch to slave clock
			if(procRes==0) {
				menu_initBlinkCounter();
				my_state=STATE_MBOOT_START;
			}
			break;
			
		case STATE_RETURN_TITLE:
			// Return to the title screen
			if(procRes==0) {
				SEQ_title_init();
				nowProcess=SEQ_title;
			}
			break;
			
		case STATE_CONNECTION_ERROR:
		case STATE_FATAL_ERROR:
		case STATE_DOWNLOAD_FAILED:
			menu_drawMessage(SearchMenuErrorMsg,1);
			menu_playErrorSFX();
			
			mf_clearRect(0x243,1,0x18);
			menu_initGameList();
			
			if(my_state==STATE_CONNECTION_ERROR)
				my_state=STATE_RESET;
			else if(my_state==STATE_DOWNLOAD_FAILED)
				my_state=STATE_INIT;
			else {
				// my_state == STATE_FATAL_ERROR
				snd_play(3);
				SEQ_title_init();
				nowProcess=SEQ_title;
			}
			break;
	}
	
	SEQ_search_mboot();
	SEQ_search_dl();
	menu_checkError(procRes);
}

static void SEQ_search_mboot(void)
{
	switch(my_state) {
		case STATE_MBOOT_START:
			rfu_NI_CHILD_setSendGameName(MbootPeer,8);
			my_state=STATE_MBOOT_POLL;
			break;
			
		case STATE_MBOOT_POLL:
			if(rfuSlotStatus_NI[MbootPeer].send.state==0x27) {
				my_state=STATE_DOWNLOAD_START;
				rfu_clearSlot(4,MbootPeer);
			}
			if(rfuSlotStatus_NI[MbootPeer].send.failCounter>0xfa) {
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				my_state=STATE_DOWNLOAD_FAILED;
				rfu_clearSlot(4,MbootPeer);
			}
			
			if(my_state!=STATE_MBOOT_POLL)
				menu_initBlinkCounter();
	}
}

static void SEQ_search_dl(void)
{
	u16 x;
	u16 *data;
	u16 checksum;
	vu16 *header;
	
	data=(u16 *)&rfuSlotStatus_NI[MbootPeer].recv;
	
	switch(my_state) {
		case STATE_DOWNLOAD_START:
			menu_blinkMessage(10,0x40);  // WAITING FOR DATA...
			if((data[0] & 0x8000)!=0) {
				snd_play(4);
				menu_initBlinkCounter();
				my_state=STATE_WAIT_DOWNLOAD_END;
				MenuBusy=TRUE;
			}
			else if(data[0]==0x49) {
				SearchMenuErrorMsg=1;  // DOWNLOAD FAILED!
				my_state=STATE_DOWNLOAD_FAILED;
			}
			break;
			
		case STATE_WAIT_DOWNLOAD_END:
			menu_blinkMessage(0xb,0x20);  // DOWNLOADING...
			if(data[0]==0x47) {
				if(my_strcmp(str_header_mboot,(u8 *)EX_WRAM+4)==0) {
					snd_play(5);
					SearchMenuTimer=2*60;
					menu_drawMessage(0xc,2);  // DOWNLOAD COMPLETED!
					my_state=STATE_DOWNLOAD_SUCCESS;
				}
				else {
					SearchMenuErrorMsg=2;  // INVALID DATA RECEIVED!
					my_state=STATE_CONNECTION_ERROR;
				}
				rfu_clearSlot(0xc,MbootPeer);
				my_changeClockMaster();
			}
			else if(data[1]>0xfa||data[0]==0x49) {
				SearchMenuErrorMsg=1;  // DOWNLOAD FAILED!
				my_state=STATE_DOWNLOAD_FAILED;
			}
			break;
			
		case STATE_DOWNLOAD_SUCCESS:
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				my_state=STATE_EXEC;
			break;
	}
	
	if(my_state==STATE_EXEC) {
		CpuCopy(&rfuLinkStatus,CPU_WRAM+0,sizeof(rfuLinkStatus),16);
		CpuCopy(str_header_mboot,CPU_WRAM+0xf0,sizeof(str_header_mboot),16);
		
		header=(vu16 *)CPU_WRAM;
		checksum=0;
		for(x=0;x<0x5a;x++)
			checksum+=*header++;
		*(vu16 *)(CPU_WRAM+0xfa)=checksum;
		
		*(vu16 *)REG_IME=0;
		*(vu8 *)SOFT_RESET_DIRECT_BUF=1;
		SoftReset(0xdc);
	}
}

#ifndef NONMATCHING
__asm__ ("
.text
	.align	2
	.globl	REQ_callback_mboot
	.type	 REQ_callback_mboot,function
	.thumb_func
REQ_callback_mboot:
	push	{r4, r5, r6, lr}
	add	sp, sp, #-4
	mov	r4, #0
	ldr	r0, .LA15
	strb	r4, [r0]
	mov	r2, sp
	add	r2, r2, #1
	mov	r0, sp
	add	r1, r2, #0
	bl	rfu_REQBN_watchLink
	mov	r0, sp
	ldrb	r0, [r0]
	cmp	r0, #0
	beq	.LA21	@cond_branch
	ldr	r2, .LA15+4
	ldr	r0, .LA15+8
	ldrb	r1, [r0]
	lsl	r0, r1, #3
	sub	r0, r0, r1
	lsl	r0, r0, #4
	add	r0, r0, r2
	ldrh	r0, [r0, #52]
	cmp	r0, #0
	beq	.LA4	@cond_branch
	ldr	r1, .LA15+12
	mov	r0, #1
	b	.LA3
.LA16:
	.align	2
.LA15:
	.word	SearchMenuErrorTimer
	.word	rfuSlotStatus_NI
	.word	MbootPeer
	.word	SearchMenuErrorMsg
.LA4:
	ldr	r1, .LA17
	mov	r0, #3
.LA3:
	strb	r0, [r1]
	ldr	r1, .LA17+4
	mov	r0, #193
	strb	r0, [r1]
	b	.LA2
.LA18:
	.align	2
.LA17:
	.word	SearchMenuErrorMsg
	.word	my_state
.LA21:
	ldr	r5, .LA19
	ldrb	r0, [r5]
	cmp	r0, #197
	bhi	.LA11	@cond_branch
	bl	rfu_REQ_recvData
	bl	rfu_REQ_sendData
.LA11:
	ldr	r6, .LA19+4
.LA14:
	bl	rfu_REQ_changeMasterSlave
	lsl	r0, r0, #16
	cmp	r0, #0
	beq	.LA2	@cond_branch
	add	r0, r4, #1
	lsl	r0, r0, #24
	lsr	r4, r0, #24
	cmp	r4, #2
	bne	.LA14	@cond_branch
	mov	r0, #4
	strb	r0, [r6]
	mov	r0, #193
	strb	r0, [r5]
.LA2:
	add	sp, sp, #4
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.LA20:
	.align	2
.LA19:
	.word	my_state
	.word	SearchMenuErrorMsg
.LAfe1:
	.size	 REQ_callback_mboot,.LAfe1-REQ_callback_mboot
");
#else
void REQ_callback_mboot(void)
{
	u8 x;
	u8 res;
	u8 res2;
	u8 res3;
	
	x=0;
	SearchMenuErrorTimer=0;
	
	rfu_REQBN_watchLink(&res,&res2,&res3);
	if(res!=0) {
		if(rfuSlotStatus_NI[MbootPeer].recv.state!=0)
			SearchMenuErrorMsg=1;
		else
			SearchMenuErrorMsg=3;
	}
	else {
		if(my_state<0xc6) {
			rfu_REQ_recvData();
			rfu_REQ_sendData();
		}
		
		for(x=0;x!=2;x++) {
			if(rfu_REQ_changeMasterSlave()==0)
				return;
		}
		SearchMenuErrorMsg=4;
	}
	
	my_state=0xc1;
}
#endif

static u8 my_strcmp(const char *str1,const char *str2)
{
	while(*str1) {
		if(*str1++!=*str2++)
			return 1;
	}
	return 0;
}

static u16 REQ_configGameData(void)
{
	return rfu_REQ_configGameData(TRUE,0,GameName,str_uname[MbootPeer]);
}

static u16 REQ_configSystem(void)
{
	return rfu_REQ_configSystem(0x1c,4,0x20);
}

static u16 REQ_startConnectParent(void)
{
	return rfu_REQ_startConnectParent(MbootBeaconID);
}

static u16 REQ_pollConnectParent(void)
{
	u16 ret;
	u8 temp[4];
	
	ret=rfu_REQ_pollConnectParent(temp,&MbootPeer);
	if(ret==0&&temp[0]!=0)
		ret=temp[0] | 0x8000;
	return ret;
}

static u16 REQ_changeMasterSlave(void)
{
	return rfu_REQ_changeMasterSlave_check(0);
}

static void my_changeClockMaster(void)
{
	rfu_REQ_changeMasterSlave_check(1);
	while(!rfu_getMasterSlave());
}

static u16 my_softReset_and_checkID(void)
{
	if(AgbRFU_softReset_and_checkID()==0x8001)
		return 0;
	else
		return 1;
}

