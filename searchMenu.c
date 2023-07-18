#include <Agb.h>
#include "myFunc.h"

#include "GameInfo.h"
#include "Mboot.h"
extern const u8 GameLogoInitial[10];
extern struct GameInfo GameList[4];
extern struct Mboot Mboot;
extern u16 (*SearchProcTable[])(void);
extern u16 Bg0Bak[32*20];
extern u16 MbootBeaconID;
extern u16 SearchMenuTimer;
extern u8 FrameCount;
extern u8 GameListBits;
extern u8 _binary_char_search_tmap_LZ_bin_start[];
extern u8 Lang;
extern u8 MbootPeer;
extern u8 MenuBusy;
extern u8 my_state;
extern u8 SearchMenuCursor;
extern u8 SearchMenuEnd;
extern u8 SearchMenuErrorMsg;
extern u8 SearchMenuErrorTimer;
extern void (*nowProcess)();

extern u16  MbootDLStart2(u8 Peer,u16 param_2);
extern u32  MbootReset(void);
extern u32  RfuMbootCfg(u32 param_1,u8 Client,void *Dest,u32 Size);
extern u32  RfuPeerUpdateFlags(u8 param_1,u8 Peer);
extern u8   RfuStrcmp(const char *Str1,const char *Str2);
extern u8   SearchMenuUpdateGames(void);
extern void FrameCountReset(void);
extern void GameListInit(void);
extern void GameNameInit(void);
extern void MenuMsgBlink(u8 Msg,u8 Rate);
extern void MenuMsgSet(u8 Msg,u16 PlttNo);
extern void RfuIntrDataTransfer(void);
extern void RfuReset(void);
extern void RfuWaitData(void);
extern void SEQ_title(void);
extern void SEQ_title_init(void);
extern void SearchMenuClearGame(void);
extern void SearchMenuDrawList(u8 Blink);
extern void SearchMenuErrorBeep(void);
extern void checkAPI_Error(u16 State);
extern void Sio32IntrProcSet(void (*Func)());
extern void SoundPlaySfx(u8 Num);
extern void WinFade(u8 Dir);

extern struct RfuBuf {
	u8 *recv;
	u8 *send;
} RfuBuf;

extern struct RfuPeer {
	u16 sub[2][0x1a];
	void *mbootDest;
	u32 mbootSize;
} RfuPeers[4];

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

static void my_drawListTitle(u16 Pos,u8 Len,u16 CharNo);
static void SEQ_search_dl(void);
static void SEQ_search_mboot(void);

void SEQ_search_init(void)
{
	u8 x;
	
	WinFade(0);
	
	mf_drawBg2_search(0x18);
	*(vu16 *)REG_BG2VOFS=-52;
	*(vu16 *)REG_BG2HOFS=-24;
	DmaClear(3,0,Bg0Bak,sizeof(Bg0Bak),16);
	LZ77UnCompVram(_binary_char_search_tmap_LZ_bin_start,(u8 *)VRAM+0x800);
	VBlankIntrWait();
	
	*(vu16 *)REG_DISPCNT|=DISP_BG1_ON;
	WinFade(1);
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
		GameList[x].beaconID=0;
	MbootReset();
	SearchMenuEnd=FALSE;
	SearchMenuCursor=0;
	SearchMenuErrorTimer=0;
	SearchMenuErrorMsg=-1;
	my_state=STATE_INIT;
	GameNameInit();
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
	
	if(Mboot.mode==0) {
		RfuReset();
		
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
			SoundPlaySfx(0);
		}
		
		// Select game
		if(key.Trg & A_BUTTON&&GameList[SearchMenuCursor].beaconID) {
			SearchMenuDrawList(FALSE);
			MbootBeaconID=GameList[SearchMenuCursor].beaconID;
			if(my_state==STATE_WAIT_SP)
				my_state=STATE_SP_END;
			else
				my_state=STATE_CP_START;
			SoundPlaySfx(2);
		}
	}
	
	if(key.Trg & B_BUTTON) {
		if(my_state==STATE_DOWNLOAD_START) {
			RfuWaitData();
			my_state=STATE_RESET;
			GameListInit();
			SoundPlaySfx(3);
		}
		else if(!SearchMenuEnd&&!MenuBusy&&
				SearchMenuErrorMsg==(u8)-1&&
				my_state!=STATE_INIT&&
				my_state!=STATE_MBOOT_POLL) {
			RfuWaitData();
			my_state=STATE_RESET;
			SearchMenuEnd=TRUE;
			SoundPlaySfx(3);
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
				FrameCountReset();
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
				MenuMsgSet(9,0);  // SELECT A GAME
			}
			else {
				MenuMsgBlink(8,0x40);  // NOW SEARCHING...
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				my_state=STATE_SP_END;
			break;
			
		case STATE_SP_END:
			// Get discovery results
			if(procRes==0) {
				if(SearchMenuTimer<60-8)
					GameListBits=SearchMenuUpdateGames();
				
				if(MbootBeaconID)
					my_state=STATE_CP_START;
				else {
					my_state=STATE_WAIT;
					SearchMenuTimer=5*60;
					FrameCount=0;
				}
			}
			break;
			
		case STATE_WAIT:
			// Allow the player to select a game
			// Restart discovery after 5 seconds
			if(GameListBits!=0) {
				MenuMsgSet(9,0);  // SELECT A GAME
				if(SearchMenuTimer>3*60)
					SearchMenuDrawList(TRUE);
				else
					SearchMenuDrawList(FALSE);
			}
			else {
				MenuMsgBlink(8,0x40);  // NOW SEARCHING...
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
				if(SearchMenuTimer>0&&RfuBuf.recv[7]==0) {
					SearchMenuClearGame();
					Sio32IntrProcSet(RfuIntrDataTransfer);
					RfuMbootCfg(0x20,MbootPeer,(u8 *)EX_WRAM,EX_WRAM_SIZE);
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
				if(RfuBuf.recv[4+MbootPeer]) {
					my_state=STATE_CHANGE_CLOCK_SLAVE;
					mf_drawString(0x6b,2,Mboot.curGame.userName);
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
				FrameCountReset();
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
			MenuMsgSet(SearchMenuErrorMsg,1);
			SearchMenuErrorBeep();
			
			mf_clearRect(0x243,1,0x18);
			GameListInit();
			
			if(my_state==STATE_CONNECTION_ERROR)
				my_state=STATE_RESET;
			else if(my_state==STATE_DOWNLOAD_FAILED)
				my_state=STATE_INIT;
			else {
				// my_state == STATE_FATAL_ERROR
				SoundPlaySfx(3);
				SEQ_title_init();
				nowProcess=SEQ_title;
			}
			break;
	}
	
	SEQ_search_mboot();
	SEQ_search_dl();
	checkAPI_Error(procRes);
}

static void SEQ_search_mboot(void)
{
	switch(my_state) {
		case STATE_MBOOT_START:
			MbootDLStart2(MbootPeer,8);
			my_state=STATE_MBOOT_POLL;
			break;
			
		case STATE_MBOOT_POLL:
			if(RfuPeers[MbootPeer].sub[0][0]==0x27) {
				my_state=STATE_DOWNLOAD_START;
				RfuPeerUpdateFlags(4,MbootPeer);
			}
			if(RfuPeers[MbootPeer].sub[0][1]>0xfa) {
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				my_state=STATE_DOWNLOAD_FAILED;
				RfuPeerUpdateFlags(4,MbootPeer);
			}
			
			if(my_state!=STATE_MBOOT_POLL)
				FrameCountReset();
	}
}

static void SEQ_search_dl(void)
{
	u16 x;
	u16 *data;
	u16 checksum;
	vu16 *header;
	
	data=RfuPeers[MbootPeer].sub[1];
	
	switch(my_state) {
		case STATE_DOWNLOAD_START:
			MenuMsgBlink(10,0x40);  // WAITING FOR DATA...
			if((data[0] & 0x8000)!=0) {
				SoundPlaySfx(4);
				FrameCountReset();
				my_state=STATE_WAIT_DOWNLOAD_END;
				MenuBusy=TRUE;
			}
			else if(data[0]==0x49) {
				SearchMenuErrorMsg=1;  // DOWNLOAD FAILED!
				my_state=STATE_DOWNLOAD_FAILED;
			}
			break;
			
		case STATE_WAIT_DOWNLOAD_END:
			MenuMsgBlink(0xb,0x20);  // DOWNLOADING...
			if(data[0]==0x47) {
				if(RfuStrcmp(GameLogoInitial,(u8 *)EX_WRAM+4)==0) {
					SoundPlaySfx(5);
					SearchMenuTimer=2*60;
					MenuMsgSet(0xc,2);  // DOWNLOAD COMPLETED!
					my_state=STATE_DOWNLOAD_SUCCESS;
				}
				else {
					SearchMenuErrorMsg=2;  // INVALID DATA RECEIVED!
					my_state=STATE_CONNECTION_ERROR;
				}
				RfuPeerUpdateFlags(0xc,MbootPeer);
				RfuWaitData();
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
		CpuCopy(&Mboot,CPU_WRAM+0,sizeof(Mboot),16);
		CpuCopy(GameLogoInitial,CPU_WRAM+0xf0,sizeof(GameLogoInitial),16);
		
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

