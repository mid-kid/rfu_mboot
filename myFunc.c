#include "myFunc.h"

#include <AgbMemoryMap.h>
#include <AgbSystemCall.h>
#include "data.h"

// define data----------------------------------

// extern data----------------------------------

// function's prototype-------------------------
static void mf_clearBg2(void);

// global data----------------------------------

// static variable------------------------------
static u8  key_rapid[4];

// const data-----------------------------------


// Reads key input
void mf_readKey()
{
	u16 ReadData=(*(vu16 *)REG_KEYINPUT ^ 0xffff);
	key.Trg=ReadData & (ReadData ^ key.Cont);    // Trigger input
	key.Cont=ReadData;                            //   Continuous input
}

// Rapid key input
void mf_rapidKey(void)
{
	u16 mask;
	u8 i;
	
	for(i=0;i<4;i++) {
		mask=0x0001<<(i+4);
		if(key.Cont & mask) {
			key_rapid[i]++;
			if(key_rapid[i]==20) {
				key.Trg|=mask;
				key_rapid[i]=15;
			}
		}
		else
			key_rapid[i]=0;
	}
}

void mf_clearGame(u16 pos)
{
	u8 x;
	u16 *bg;
	
	bg=Bg0Bak+pos;
	for(x=0;x<8;x++)
		*bg++=0x142;
}

void mf_drawBg2_title(void)
{
	u8 i;
	u16 charNo;
	vu16 *bg;
	
	mf_clearBg2();
	bg=(vu16 *)(BG_VRAM+0x1000);
	
	charNo=0x2143;
	for(i=0;i<4;i++)
		*bg++=charNo++;
	for(i=0;i<5;i++)
		*bg++=charNo;
	charNo=(charNo-1) | 0x400;
	for(i=0;i<4;i++)
		*bg++=charNo--;
}

void mf_drawBg2_search(u8 len)
{
	u8 i;
	vu16 *bg;
	
	mf_clearBg2();
	bg=(vu16 *)(BG_VRAM+0x1000);
	for(i=0;i<len;i++) {
		bg[32*1]=0x113;
		bg[32*0]=0x113;
		bg++;
	}
}

static void mf_clearBg2(void)
{
	u8 i;
	vu16 *bg;
	
	bg=(vu16 *)(BG_VRAM+0x1000);
	for(i=0;i<0x40;i++)
		*bg++=0;
}

// 1 byte character string data display (single characters in 0x00 to 0xff are character strings drawn with 1 byte)
u16 *mf_drawString(u16 pos,u16 color,const u8 *str)
{
	u16 *dst;
	
	dst=Bg0Bak+pos;
	while(*str)
		*dst++=(u16)*str++ | color << 12;
	return dst;
}

// Clears rectangle BG
void mf_clearRect(u16 topPos,u8 height,u8 width)
{
	u8 i,j;
	u16 *leftTop;
	
	for(i=0;i<height;i++) {
		leftTop=Bg0Bak+topPos+(i<<5);
		for(j=0;j<width;j++)
			*leftTop++=0x0020;
	}
}

// 3 second wait
void mf_wait3sec(void)
{
	u8 i;
	
	for(i=0;i<3*60;i++)
		VBlankIntrWait();
}

void mf_winInit(void)
{
	*(vu16 *)REG_WIN0H=0xff;
	*(vu16 *)REG_WIN0V=0xff;
	*(vu16 *)REG_WIN0=0x3f;
	*(vu16 *)REG_WINOUT=0;
	*(vu16 *)REG_BLDCNT=0x2857;
	*(vu16 *)REG_BLDALPHA=0x1f00;
}

void mf_winFade(u8 dir)
{
	u16 i;
	u16 val;
	
	if(dir==0)
		*(vu16 *)REG_DISPCNT&=0xfbff;
	
	i=0;
	while(i<=0x10) {
		VBlankIntrWait();
		if(i==0x10)
			val=0;
		else {
			//val=0x10-i;
			val=(~i & 0xf)+1;
		}
		if(dir)
			*(vu16 *)REG_BLDALPHA=val<<8 | i;
		else
			*(vu16 *)REG_BLDALPHA=val | i<<8;
		i++;
		VBlankIntrWait();
	}
}

