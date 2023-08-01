#include <Agb.h>
#include "GameInfo.h"
#include "myFunc.h"
#include "rfuLinkStatus.h"

extern void SoundPlaySfx(u8 Num);
extern void menu_drawGame(u16 Pos,struct GameInfo *Game);
extern void menu_drawMessage(u8 Msg,u16 PlttNo);
extern void mf_clearGame(int Pos);
extern const struct MenuMsg *MenuMsg;

extern struct Sound {
	u16 *basePtr;
	u16 *playPtr;
	u8 time;
	u8 sfxNum;
} Sound;

struct MenuMsg {
	const char *const *msg;
	const u16 *pos;
};

extern struct GameInfo GameList[4];
extern struct rfuLinkStatus rfuLinkStatus;
extern u8 GameListBitsNew;
extern u8 GameName[14];
extern u8 MenuBusy;
extern u8 SearchMenuCursor;
extern u8 SearchMenuErrorMsg;
extern u8 blink_counter;
extern u8 my_state;

static const u8 str_my_gname_mboot[]="RFU-MB-DL";
const u8 str_header_mboot[]="RFU-MBOOT";

u8 menu_drawGameList(void)
{
	u8 x;
	u8 y;
	u8 GamesOld;
	u8 GamesNew;
	u16 Pos;
	
	// Find out which games are already in the list, and which ones aren't
	GamesOld=0;
	GamesNew=0;
	for(x=0;x<rfuLinkStatus.gamesCount;x++) {
		if(rfuLinkStatus.games[x].beaconID&&
				rfuLinkStatus.games[x].isMultiboot&&
				rfuLinkStatus.games[x].playerNum<4) {
			for(y=0;y<4;y++) {
				if(GameList[y].beaconID!=rfuLinkStatus.games[x].beaconID)
					continue;
				GamesOld|=1<<y;
				break;
			}
			if(y==4)
				GamesNew|=1<<x;
		}
	}
	
	// Remove any games that don't exist anymore from the list
	for(x=0;x<4;x++)
		if(!(GamesOld & 1<<x))
			GameList[x].beaconID=0;
	
	// Copy new games into the games list
	GameListBitsNew=0;
	for(x=0;x<4;x++) {
		if(!(GamesNew & 1<<x))
			continue;
		for(y=0;y<4;y++) {
			if(GameList[y].beaconID)
				continue;
			CpuCopy(&rfuLinkStatus.games[x],&GameList[y],
				sizeof(struct GameInfo),16);
			GameListBitsNew|=1<<y;
			break;
		}
	}
	
	// If any new game was found, play a ditty
	if(GameListBitsNew!=0)
		SoundPlaySfx(1);
	
	// Draw the game list
	mf_clearRect(0xe3,8,0x19);
	Pos=0xe3;
	for(x=0;x<4;x++) {
		if(GameList[x].beaconID!=0)
			menu_drawGame(Pos,&GameList[x]);
		Pos+=0x40;
	}
	return GamesOld | GamesNew;
}

void menu_clearGameList(void)
{
	u8 x;
	
	for(x=0;x<4;x++) {
		if(x==SearchMenuCursor)
			continue;
		GameList[x].beaconID=0;
		mf_clearRect(x*0x40+0xe3,1,0x19);
	}
}

void menu_initGameList(void)
{
	MenuBusy=0;
	mf_clearRect(0xe3,8,0x19);
	
	CpuClear(0,GameList,sizeof(GameList),16);
}

void menu_blinkGame(u8 Blink)
{
	u8 x;
	u16 pos;
	
	for(pos=0xe3,x=0;x<4;pos+=0x40,x++) {
		if(!(GameListBitsNew & 1<<x))
			continue;
		
		if(!Blink||(blink_counter & 0x20)==0)
			menu_drawGame(pos,GameList+x);
		else
			mf_clearRect(pos,1,0x19);
	}
	
	blink_counter++;
}

void menu_drawGame(u16 Pos,struct GameInfo *Game)
{
	if(!Game->gameName[0])
		mf_clearGame(Pos);
	else
		mf_drawString(Pos,0,Game->gameName);
	
	if(!Game->userName[0])
		mf_clearGame(Pos+15);
	else
		mf_drawString(Pos+15,0,Game->userName);
}

void menu_checkError(u16 State)
{
	if(!State)
		return;
	
	if(my_state==10)
		my_state=0xc2;
	else if(my_state==0)
		my_state=0xc5;
	else
		my_state=0xc1;
	
	SearchMenuErrorMsg=4;
}

void menu_playErrorSFX(void)
{
	u8 x;
	u8 count;
	
	count=0;
	SoundPlaySfx(6);
	for(x=0;x<3*60;x++) {
		VBlankIntrWait();
		
		// Play the SFX twice
		if(Sound.time==(u8)-1&&count<1) {
			count++;
			SoundPlaySfx(6);
		}
	}
}

void menu_drawMessage(u8 Msg,u16 PlttNo)
{
	mf_clearRect(MenuMsg->pos[Msg] & -0x20,2,0x20);
	mf_drawString(MenuMsg->pos[Msg],PlttNo,MenuMsg->msg[Msg]);
}

void menu_initBlinkCounter(void)
{
	blink_counter=0;
}

void menu_blinkMessage(u8 Msg,u8 Rate)
{
	if(Msg==6)
		mf_clearRect(MenuMsg->pos[6],2,0x20);
	else
		mf_clearRect(0x200,2,0x20);
	
	if((blink_counter & Rate)==0)
		menu_drawMessage(Msg,0);
	
	blink_counter++;
}

void menu_initGameName(void)
{
	u8 i;
	u8 sum;
	vu8 *cst;
	
	// Calculate rom checksum
	sum=0x19;
	cst=(vu8 *)ROM_BANK0+0xa0;
	while(cst!=(vu8 *)ROM_BANK0+0xbd)
		sum+=*cst++;
	sum=~sum+1; // sum=-sum;
	
	// Initialize the game name
	for(i=0;i<sizeof(GameName);i++) {
		if(i<sizeof(str_my_gname_mboot))
			GameName[i]=str_my_gname_mboot[i];
		else
			GameName[i]=0;
	}
	
	// If a valid game is inserted, append the game code
	if(*cst==sum) {
		cst=(vu8 *)(ROM_BANK0 + 0xac);
		for(i=sizeof(str_my_gname_mboot);i<sizeof(GameName);i++)
			GameName[i]=*cst++;
	}
}

