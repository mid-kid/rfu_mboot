#include <Agb.h>
#include "myFunc.h"

extern u16 Bg0Bak[32*20];

KEY_DATA key;
static u8  key_rapid[4];

// Reads key input
void mf_readKey()
{
	u16 ReadData = (*(vu16 *)REG_KEYINPUT ^ 0xffff);
	key.Trg  = ReadData & (ReadData ^ key.Cont);    // Trigger input
	key.Cont = ReadData;                            //   Continuous input
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

void mf_clearGame(u16 Pos)
{
	u8 x;
	u16 *bg;

	bg = Bg0Bak + Pos;
	for (x = 0; x < 8; x++) {
		*bg++ = 0x142;
	}
}

void mf_clearBg2(void);

void mf_drawBg2_main(void)
{
	u8 i;
	u16 charNo;
	vu16 *bg;

	mf_clearBg2();
	bg = (vu16 *)(BG_VRAM + 0x1000);

	charNo = 0x2143;
	for (i = 0; i < 4; i++) *bg++ = charNo++; 
	for (i = 0; i < 5; i++) *bg++ = charNo; 
	charNo = (charNo - 1) | 0x400;
	for (i = 0; i < 4; i++) *bg++ = charNo--; 
}

void mf_drawBg2_search(u8 Len)
{
	u8 i;
	vu16 *bg;

	mf_clearBg2();
	bg = (vu16 *)(BG_VRAM + 0x1000);
	for (i = 0; i < Len; i++) {
		bg[32*1] = 0x113;
		bg[32*0] = 0x113;
		bg++;
	}
}

void mf_clearBg2(void)
{
	u8 i;
	vu16 *bg;

	bg = (vu16 *)(BG_VRAM + 0x1000);
	for (i = 0; i < 0x40; i++) *bg++ = 0; 
}

// 1 byte character string data display (single characters in 0x00 to 0xff are character strings drawn with 1 byte)
u16 *mf_drawString (u16 pos, u16 color, const u8 *str)
{
	u16 *dst;
	
	dst=Bg0Bak+pos;
	while(*str)
		*dst++ = (u16)*str++ | color << 12;
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
