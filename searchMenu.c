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
extern u8 LZ_450c[];
extern u8 Lang;
extern u8 MbootPeer;
extern u8 MenuBusy;
extern u8 MenuState;
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
extern void SearchMenuErrorCheck(u16 State);
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

enum SearchMenuState {
	// Correspond to entries in SearchProcTable
	SEARCH_START,
	SEARCH_RADIOCFG,
	SEARCH_SETGAMEINFO,
	SEARCH_DISCOSTART,
	SEARCH_0x1d,
	SEARCH_DISCORES,
	SEARCH_CONNECT,
	SEARCH_CONNCHECK,
	SEARCH_CONNREADY,
	SEARCH_WAITDATA,
	SEARCH_BOOT,
	SEARCH_END,
	SEARCH_STATUS,
	SEARCH_0x13,
	
	// Standalone states
	SEARCH_SELECT_DISCO = 0x84,
	SEARCH_MBOOT_START = 0x90,
	SEARCH_MBOOT_START_CHECK = 0x91,
	SEARCH_SELECT = 0x92,
	SEARCH_MBOOT_DL_START = 0xa0,
	SEARCH_MBOOT_DL = 0xa1,
	SEARCH_ERROR_RESTART = 0xc1,
	SEARCH_ERROR_END = 0xc2,
	SEARCH_ERROR_REBOOT = 0xc5,
	SEARCH_MBOOT_DL_COMPLETE = 0xc6,
	SEARCH_MBOOT_EXEC = 0xc7
};

static void SearchMenuDrawListTitle(u16 Pos,u8 Len,u16 CharNo);
static void SearchMenuMbootDL(void);
static void SearchMenuMbootStart(void);

void SearchMenuInit(void)
{
	u8 x;
	
	WinFade(0);
	
	mf_drawBg2_search(0x18);
	*(vu16 *)REG_BG2VOFS=-52;
	*(vu16 *)REG_BG2HOFS=-24;
	DmaClear(3,0,Bg0Bak,sizeof(Bg0Bak),16);
	LZ77UnCompVram(&LZ_450c,(void *)0x6000800);
	VBlankIntrWait();
	
	*(vu16 *)REG_DISPCNT|=DISP_BG1_ON;
	WinFade(1);
	*(vu16 *)REG_DISPCNT|=DISP_BG2_ON;
	
	if(Lang==0) {
		SearchMenuDrawListTitle(0xa6,8,0x151);
		SearchMenuDrawListTitle(0xb2,8,0x159);
	}
	else {
		SearchMenuDrawListTitle(0xa7,6,0x161);
		SearchMenuDrawListTitle(0xb3,7,0x167);
	}
	
	for(x=0;x<4;x++)
		GameList[x].beaconID=0;
	MbootReset();
	SearchMenuEnd=FALSE;
	SearchMenuCursor=0;
	SearchMenuErrorTimer=0;
	SearchMenuErrorMsg=-1;
	MenuState=SEARCH_BOOT;
	GameNameInit();
}

extern u16 Bg0Bak[32*20];

static void SearchMenuDrawListTitle(u16 Pos,u8 Len,u16 CharNo)
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

void SearchMenu(void)
{
	u16 procRes=0;
	
	if(Mboot.mode==0) {
		RfuReset();
		
		if(MenuState!=SEARCH_MBOOT_DL_COMPLETE) {
			*(vu16 *)REG_IME=0;
			SearchMenuErrorTimer++;
			if(SearchMenuErrorTimer>4*60) {
				SearchMenuErrorTimer=0;
				MenuState=SEARCH_BOOT;
				SearchMenuErrorMsg=4;  // ERROR OCCURRED!
			}
			*(vu16 *)REG_IME=1;
		}
	}
	
	if(MenuState==SEARCH_SELECT_DISCO||MenuState==SEARCH_SELECT) {
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
			if(MenuState==SEARCH_SELECT_DISCO)
				MenuState=SEARCH_DISCORES;
			else
				MenuState=SEARCH_CONNECT;
			SoundPlaySfx(2);
		}
	}
	
	if(key.Trg & B_BUTTON) {
		if(MenuState==SEARCH_MBOOT_DL_START) {
			RfuWaitData();
			MenuState=SEARCH_START;
			GameListInit();
			SoundPlaySfx(3);
		}
		else if(!SearchMenuEnd&&
				!MenuBusy&&
				SearchMenuErrorMsg==(u8)-1&&
				MenuState!=SEARCH_BOOT&&
				MenuState!=SEARCH_MBOOT_START_CHECK) {
			RfuWaitData();
			MenuState=SEARCH_START;
			SearchMenuEnd=TRUE;
			SoundPlaySfx(3);
		}
	}
	
	if(!(MenuState & 0x80))
		procRes=SearchProcTable[MenuState]();
	
	switch(MenuState) {
		case SEARCH_BOOT:
			// Resets and boots the adapter
			if(procRes==0)
				MenuState=SEARCH_START;
			break;
			
		case SEARCH_START:
			if(procRes==0) {
				SearchMenuErrorMsg=-1;
				if(SearchMenuEnd!=FALSE)
					MenuState=SEARCH_END;
				else {
					MenuState=SEARCH_RADIOCFG;
					mf_clearRect(0x6b,1,8);
				}
			}
			break;
			
		case SEARCH_RADIOCFG:
			if(procRes==0) {
				MenuState=SEARCH_DISCOSTART;
				FrameCountReset();
				GameListBits=0;
			}
			break;
			
		case SEARCH_DISCOSTART:
			// Starts game discovery
			if(procRes==0) {
				MenuState=SEARCH_SELECT_DISCO;
				MbootBeaconID=0;
				SearchMenuTimer=1*60;
			}
			break;
			
		case SEARCH_SELECT_DISCO:
			// Wait one second for discovery to finish
			if(GameListBits) {
				MenuMsgSet(9,0);  // SELECT A GAME
			}
			else {
				MenuMsgBlink(8,0x40);  // NOW SEARCHING...
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				MenuState=SEARCH_DISCORES;
			break;
			
		case SEARCH_DISCORES:
			// Get discovery results
			if(procRes==0) {
				if(SearchMenuTimer<60-8)
					GameListBits=SearchMenuUpdateGames();
				
				if(MbootBeaconID)
					MenuState=SEARCH_CONNECT;
				else {
					MenuState=SEARCH_SELECT;
					SearchMenuTimer=5*60;
					FrameCount=0;
				}
			}
			break;
			
		case SEARCH_SELECT:
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
				MenuState=SEARCH_DISCOSTART;
			break;
			
		case SEARCH_CONNECT:
			// Start connecting to a game
			if(procRes==0) {
				mf_clearRect(0x200,2,0x20);
				SearchMenuTimer=2*60;
				MenuState=SEARCH_CONNCHECK;
			}
			else if(procRes==0x900) {
				procRes=0;
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				MenuState=SEARCH_ERROR_RESTART;
			}
			break;
			
		case SEARCH_CONNCHECK:
			// Wait a maximum of two seconds for the connection to complete
			if(procRes==0) {
				MenuState=SEARCH_CONNREADY;
				break;
			}
			
			SearchMenuTimer--;
			if(procRes & 0x8000) {
				if((procRes & 0xff)==2)
					SearchMenuTimer=0;
				procRes=0;
			}
			if(SearchMenuTimer==0)
				MenuState=SEARCH_CONNREADY;
			break;
			
		case SEARCH_CONNREADY:
			// Initialize multiboot download
			if(procRes==0) {
				if(SearchMenuTimer>0&&RfuBuf.recv[7]==0) {
					SearchMenuClearGame();
					Sio32IntrProcSet(RfuIntrDataTransfer);
					RfuMbootCfg(0x20,MbootPeer,(u8 *)EX_WRAM,EX_WRAM_SIZE);
					MenuState=SEARCH_SETGAMEINFO;
				}
				else {
					SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
					MenuState=SEARCH_ERROR_RESTART;
				}
			}
			break;
			
		case SEARCH_SETGAMEINFO:
			// Set the timer for the next step
			if(procRes==0) {
				SearchMenuTimer=0.5*60;
				MenuState=SEARCH_STATUS;
			}
			break;
			
		case SEARCH_STATUS:
			// Check if peer is still connected
			if(procRes==0) {
				if(RfuBuf.recv[4+MbootPeer]) {
					MenuState=SEARCH_WAITDATA;
					mf_drawString(0x6b,2,Mboot.curGame.userName);
					break;
				}
			}
			
			SearchMenuTimer--;
			if(SearchMenuTimer==0) {
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				MenuState=SEARCH_ERROR_RESTART;
			}
			break;
			
		case SEARCH_WAITDATA:
			if(procRes==0) {
				FrameCountReset();
				MenuState=SEARCH_MBOOT_START;
			}
			break;
			
		case SEARCH_END:
			// Return to main menu
			if(procRes==0) {
				SEQ_title_init();
				nowProcess=SEQ_title;
			}
			break;
			
		case SEARCH_ERROR_RESTART:
		case SEARCH_ERROR_END:
		case SEARCH_ERROR_REBOOT:
			MenuMsgSet(SearchMenuErrorMsg,1);
			SearchMenuErrorBeep();
			
			mf_clearRect(0x243,1,0x18);
			GameListInit();
			
			if(MenuState==SEARCH_ERROR_RESTART)
				MenuState=SEARCH_START;
			else if(MenuState==SEARCH_ERROR_REBOOT)
				MenuState=SEARCH_BOOT;
			else {
				// MenuState == SEARCH_ERROR_END
				SoundPlaySfx(3);
				SEQ_title_init();
				nowProcess=SEQ_title;
			}
			break;
	}
	
	SearchMenuMbootStart();
	SearchMenuMbootDL();
	SearchMenuErrorCheck(procRes);
}

static void SearchMenuMbootStart(void)
{
	switch(MenuState) {
		case SEARCH_MBOOT_START:
			MbootDLStart2(MbootPeer,8);
			MenuState=SEARCH_MBOOT_START_CHECK;
			break;
			
		case SEARCH_MBOOT_START_CHECK:
			if(RfuPeers[MbootPeer].sub[0][0]==0x27) {
				MenuState=SEARCH_MBOOT_DL_START;
				RfuPeerUpdateFlags(4,MbootPeer);
			}
			if(RfuPeers[MbootPeer].sub[0][1]>0xfa) {
				SearchMenuErrorMsg=0;  // CONNECTION ATTEMPT FAILED!
				MenuState=SEARCH_ERROR_REBOOT;
				RfuPeerUpdateFlags(4,MbootPeer);
			}
			
			if(MenuState!=SEARCH_MBOOT_START_CHECK)
				FrameCountReset();
	}
}

static void SearchMenuMbootDL(void)
{
	u16 x;
	u16 *data;
	u16 checksum;
	vu16 *header;
	
	data=RfuPeers[MbootPeer].sub[1];
	
	switch(MenuState) {
		case SEARCH_MBOOT_DL_START:
			MenuMsgBlink(10,0x40);  // WAITING FOR DATA...
			if((data[0] & 0x8000)!=0) {
				SoundPlaySfx(4);
				FrameCountReset();
				MenuState=SEARCH_MBOOT_DL;
				MenuBusy=TRUE;
			}
			else if(data[0]==0x49) {
				SearchMenuErrorMsg=1;  // DOWNLOAD FAILED!
				MenuState=SEARCH_ERROR_REBOOT;
			}
			break;
			
		case SEARCH_MBOOT_DL:
			MenuMsgBlink(0xb,0x20);  // DOWNLOADING...
			if(data[0]==0x47) {
				if(RfuStrcmp(GameLogoInitial,(u8 *)EX_WRAM+4)==0) {
					SoundPlaySfx(5);
					SearchMenuTimer=2*60;
					MenuMsgSet(0xc,2);  // DOWNLOAD COMPLETED!
					MenuState=SEARCH_MBOOT_DL_COMPLETE;
				}
				else {
					SearchMenuErrorMsg=2;  // INVALID DATA RECEIVED!
					MenuState=SEARCH_ERROR_RESTART;
				}
				RfuPeerUpdateFlags(0xc,MbootPeer);
				RfuWaitData();
			}
			else if(data[1]>0xfa||data[0]==0x49) {
				SearchMenuErrorMsg=1;  // DOWNLOAD FAILED!
				MenuState=SEARCH_ERROR_REBOOT;
			}
			break;
			
		case SEARCH_MBOOT_DL_COMPLETE:
			SearchMenuTimer--;
			if(SearchMenuTimer==0)
				MenuState=SEARCH_MBOOT_EXEC;
			break;
	}
	
	if(MenuState==SEARCH_MBOOT_EXEC) {
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

