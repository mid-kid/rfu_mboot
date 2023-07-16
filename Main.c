#include <Agb.h>

// define data------------------------------------------
#define BG0_SCBLK				0
#define BG1_SCBLK				1
#define BG2_SCBLK				2
#define BG3_SCBLK				3
#define BG0_CHBLK				1
#define BG1_CHBLK				1
#define BG2_CHBLK				1
#define BG3_CHBLK				1

// exten data-------------------------------------------
extern u8 LZ_460c[];
extern u8 LZ_43c8[];
extern u8 LZ_4420[];
extern u8 MainMenuFadeOut;
extern u8 Lang;
extern void (*nowProcess)();
extern u8 bss_start[];
extern u8 bss_end[];

// function's prototype---------------------------------
extern void intr_main(void);
extern void SoundInit(void);
extern void WinInit(void);
extern void RfuInit(void);
extern void RfuSetUnk04(u8 param_1);
extern void SEQ_title_init(void);
extern void SEQ_title(void);
extern void mf_readKey(void);

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
	
	LZ77UnCompVram(LZ_460c,(u8 *)VRAM+0x4000);
	LZ77UnCompVram(LZ_43c8,(u8 *)PLTT);
	LZ77UnCompVram(LZ_4420,(u8 *)VRAM+0x1800);
	
	SoundInit();
	WinInit();
	
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
	RfuInit();
	
// INTERRUPT ENABLE
	*(vu16 *)REG_IE=SIO_INTR_FLAG | V_BLANK_INTR_FLAG;
	*(vu16 *)REG_STAT=STAT_V_BLANK_IF_ENABLE;
	*(vu16 *)REG_IME=1;
	
	RfuSetUnk04(8);
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

