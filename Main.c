#include "rfuDefine.h"

#include <AgbDefine.h>
#include <AgbMemoryMap.h>
#include <AgbSystemCall.h>
#include <AgbMacro.h>
#include "myFunc.h"
#include "data.h"
#include "sound.h"

// define data------------------------------------------
#define BG0_SCBLK				0
#define BG1_SCBLK				1
#define BG2_SCBLK				2
#define BG3_SCBLK				3
#define BG0_CHBLK				1
#define BG1_CHBLK				1
#define BG2_CHBLK				1
#define BG3_CHBLK				1

// extern data------------------------------------------
extern u8 _binary_char_pals_LZ_bin_start[];
extern u8 _binary_char_tiles_LZ_bin_start[];
extern u8 _binary_char_tmap_LZ_bin_start[];
extern u8 bss_end[];
extern u8 bss_start[];
extern void intr_main(void);

// function's prototype---------------------------------
static void MenuMsgInit(void);

// global variable -------------------------------------

// static variable -------------------------------------

// const data  -----------------------------------------


//----------------------------------------------
//
// Main
//
//----------------------------------------------

void AgbMain(void)
{
	*(vu16 *)REG_IME=0;
// REGISTER initialize
	*(vu16 *)REG_WAITCNT=CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT;
	
// RAM clear & initialize
	RegisterRamReset(RESET_EX_WRAM_FLAG | RESET_PLTT_FLAG | RESET_VRAM_FLAG | RESET_OAM_FLAG);
	DmaClear(3,0,bss_start,(u32)(bss_end-bss_start),32);
	*(vu32 *)INTR_VECTOR_BUF=(vu32)intr_main;
	
	LZ77UnCompVram(_binary_char_tiles_LZ_bin_start,(u8 *)VRAM+0x4000);
	LZ77UnCompVram(_binary_char_pals_LZ_bin_start,(u8 *)PLTT);
	LZ77UnCompVram(_binary_char_tmap_LZ_bin_start,(u8 *)VRAM+0x1800);
	
	snd_init();
	mf_winInit();
	
// BG SET
	*(vu16 *)REG_BG0CNT=BG_SCREEN_SIZE_0
		| BG0_SCBLK<<BG_SCREEN_BASE_SHIFT
		| BG_COLOR_16
		| BG0_CHBLK<<BG_CHAR_BASE_SHIFT
		| BG_PRIORITY_0;
	*(vu16 *)REG_BG1CNT=BG_SCREEN_SIZE_0
		| BG1_SCBLK<<BG_SCREEN_BASE_SHIFT
		| BG_COLOR_16
		| BG1_CHBLK<<BG_CHAR_BASE_SHIFT
		| BG_PRIORITY_1;
	*(vu16 *)REG_BG2CNT=BG_SCREEN_SIZE_0
		| BG2_SCBLK<<BG_SCREEN_BASE_SHIFT
		| BG_COLOR_16
		| BG2_CHBLK<<BG_CHAR_BASE_SHIFT
		| BG_PRIORITY_2;
	*(vu16 *)REG_BG3CNT=BG_SCREEN_SIZE_0
		| BG3_SCBLK<<BG_SCREEN_BASE_SHIFT
		| BG_COLOR_16
		| BG3_CHBLK<<BG_CHAR_BASE_SHIFT
		| BG_PRIORITY_3;
	
// DISP ON
	*(vu16 *)REG_DISPCNT=DISP_BG0_ON | DISP_BG3_ON;
	
// Initialize
	rfu_initializeAPI();
	
// INTERRUPT ENABLE
	*(vu16 *)REG_IE=SIO_INTR_FLAG | V_BLANK_INTR_FLAG;
	*(vu16 *)REG_STAT=STAT_V_BLANK_IF_ENABLE;
	*(vu16 *)REG_IME=1;
	
	rfu_setWatchInterval(8);
	MainMenuFadeOut=FALSE;
	Lang=0;
	
	SEQ_title_init();
	nowProcess=SEQ_title;
	
	/***************************************
	  Main Loop
	***************************************/
	while(1) {
		VBlankIntrWait();
		
		mf_readKey();
		
		nowProcess();
	}
}


//----------------------------------------------
//
// Title
//
//----------------------------------------------
void SEQ_title_init(void)
{
	u16 i;
	u16 charNo;
	u16 *bg;
	
	if(MainMenuFadeOut) {
		mf_clearRect(0x80,2,0x20);
		mf_winFade(0);
	}
	*(vu16 *)REG_DISPCNT&=~DISP_BG1_ON;
	
	CpuClear(0,Bg0Bak,sizeof(Bg0Bak),16);
	MenuMsgInit();
	
	mf_drawString(0xcb,0,str_lang_en);
	bg=Bg0Bak+(9*32+13);
	charNo=0x10f;
	for(i=0;i<3;i++)
		*bg++=charNo++;
	
	mf_drawBg2_title();
	*(vu16 *)REG_BG2VOFS=~(56+Lang*24-1);
	*(vu16 *)REG_BG2HOFS=~(65-1);
	
	mf_winFade(1);
	*(vu16 *)REG_DISPCNT|=DISP_BG2_ON;
	
	my_state=0xc0;
	menu_initBlinkCounter();
	MainMenuFadeOut=TRUE;
	MenuBusy=FALSE;
}

static void MenuMsgInit(void)
{
	MenuMsg=MenuMsgInitial+Lang;
}

void SEQ_title(void)
{
	menu_blinkMessage(6,0x40);
	
	if(key.Trg & (D_KEY | U_KEY)) {
		snd_play(0);
		Lang^=1;
		*(vu16 *)REG_BG2VOFS=~(56+Lang*24-1);
	}
	
	if(key.Trg & A_BUTTON) {
		MenuMsgInit();
		snd_play(2);
		SEQ_search_init();
		nowProcess=SEQ_search;
	}
}

