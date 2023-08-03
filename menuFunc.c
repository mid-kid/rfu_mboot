#include "rfuDefine.h"

#include <Agb.h>
#include <AgbRFU_LL.h>
#include "myFunc.h"
#include "data.h"
#include "sound.h"

// This function is not delcared in myFunc.h, giving it an implicit prototype
extern void mf_clearGame(int pos);

static void menu_STC_drawGame(u16 pos,rfuTgtData *tgt);

u8 menu_drawGameList(void)
{
	u8 x;
	u8 y;
	u8 GamesOld;
	u8 GamesNew;
	u16 pos;
	
	// Find out which games are already in the list, and which ones aren't
	GamesOld=0;
	GamesNew=0;
	for(x=0;x<rfuLinkStatus.findParentCount;x++) {
		if(rfuLinkStatus.partner[x].id&&
				rfuLinkStatus.partner[x].mboot_flag&&
				rfuLinkStatus.partner[x].slot<4) {
			for(y=0;y<4;y++) {
				if(GameList[y].id!=rfuLinkStatus.partner[x].id)
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
			GameList[x].id=0;
	
	// Copy new games into the games list
	GameListBitsNew=0;
	for(x=0;x<4;x++) {
		if(!(GamesNew & 1<<x))
			continue;
		for(y=0;y<4;y++) {
			if(GameList[y].id)
				continue;
			CpuCopy(&rfuLinkStatus.partner[x],GameList+y,sizeof(rfuTgtData),16);
			GameListBitsNew|=1<<y;
			break;
		}
	}
	
	// If any new game was found, play a ditty
	if(GameListBitsNew!=0)
		snd_play(1);
	
	// Draw the game list
	mf_clearRect(0xe3,8,0x19);
	pos=0xe3;
	for(x=0;x<4;x++) {
		if(GameList[x].id!=0)
			menu_STC_drawGame(pos,GameList+x);
		pos+=0x40;
	}
	return GamesOld | GamesNew;
}

void menu_clearGameList(void)
{
	u8 x;
	
	for(x=0;x<4;x++) {
		if(x==SearchMenuCursor)
			continue;
		GameList[x].id=0;
		mf_clearRect(x*0x40+0xe3,1,0x19);
	}
}

void menu_initGameList(void)
{
	MenuBusy=0;
	mf_clearRect(0xe3,8,0x19);
	
	CpuClear(0,GameList,sizeof(GameList),16);
}

void menu_blinkGame(u8 blink)
{
	u8 x;
	u16 pos;
	
	for(pos=0xe3,x=0;x<4;pos+=0x40,x++) {
		if(!(GameListBitsNew & 1<<x))
			continue;
		
		if(!blink||(blink_counter & 0x20)==0)
			menu_STC_drawGame(pos,GameList+x);
		else
			mf_clearRect(pos,1,0x19);
	}
	
	blink_counter++;
}

static void menu_STC_drawGame(u16 pos,rfuTgtData *tgt)
{
	if(!tgt->gname[0])
		mf_clearGame(pos);
	else
		mf_drawString(pos,0,tgt->gname);
	
	if(!tgt->uname[0])
		mf_clearGame(pos+15);
	else
		mf_drawString(pos+15,0,tgt->uname);
}

void menu_checkError(u16 state)
{
	if(!state)
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
	snd_play(6);
	for(x=0;x<3*60;x++) {
		VBlankIntrWait();
		
		// Play the SFX twice
		if(sndStatic.time==(u8)-1&&count<1) {
			count++;
			snd_play(6);
		}
	}
}

void menu_drawMessage(u8 msg,u16 pltt)
{
	mf_clearRect(MenuMsg->pos[msg] & -0x20,2,0x20);
	mf_drawString(MenuMsg->pos[msg],pltt,MenuMsg->msg[msg]);
}

void menu_initBlinkCounter(void)
{
	blink_counter=0;
}

void menu_blinkMessage(u8 msg,u8 rate)
{
	if(msg==6)
		mf_clearRect(MenuMsg->pos[6],2,0x20);
	else
		mf_clearRect(0x200,2,0x20);
	
	if((blink_counter & rate)==0)
		menu_drawMessage(msg,0);
	
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

