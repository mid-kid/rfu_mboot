#include <Agb.h>

extern void intr_main(void);
extern void SoundInit(void);
extern void WinInit(void);
extern void RfuInit(void);
extern void RfuSetUnk04(u8 param_1);
extern void MainMenuInit(void);
extern void MainMenu(void);
extern void KeyRead(void);
extern u8 LZ_460c[];
extern u8 LZ_43c8[];
extern u8 LZ_4420[];
extern u8 MainMenuFadeOut;
extern u8 Lang;
extern void (*Proc)();
extern u8 bss_start[];
extern u8 bss_end[];

void DemoMain(void)
{
    *(vu16 *)REG_IME = 0;
    *(vu16 *)REG_WAITCNT = CST_ROM0_2ND_1WAIT | CST_ROM0_1ST_3WAIT;
    RegisterRamReset(RESET_ALL_RAM_FLAG & ~RESET_CPU_WRAM_FLAG);
    DmaClear(3, 0, bss_start, (u32)(bss_end - bss_start), 32);
    IntrAddrSet(intr_main);

    LZ77UnCompVram(LZ_460c, (void *)0x6004000);
    LZ77UnCompVram(LZ_43c8, (void *)0x5000000);
    LZ77UnCompVram(LZ_4420, (void *)0x6001800);
    SoundInit();
    WinInit();

    *(vu16 *)REG_BG0CNT = 
        0 << BG_SCREEN_BASE_SHIFT |
        1 << BG_CHAR_BASE_SHIFT |
        0 << BG_PRIORITY_SHIFT;
    *(vu16 *)REG_BG1CNT =
        1 << BG_SCREEN_BASE_SHIFT | 
        1 << BG_CHAR_BASE_SHIFT |
        1 << BG_PRIORITY_SHIFT;
    *(vu16 *)REG_BG2CNT =
        2 << BG_SCREEN_BASE_SHIFT |
        1 << BG_CHAR_BASE_SHIFT |
        2 << BG_PRIORITY_SHIFT;
    *(vu16 *)REG_BG3CNT =
        3 << BG_SCREEN_BASE_SHIFT |
        1 << BG_CHAR_BASE_SHIFT |
        3 << BG_PRIORITY_SHIFT;
    *(vu16 *)REG_DISPCNT = DISP_BG0_ON | DISP_BG3_ON;

    RfuInit();
    *(vu16 *)REG_IE = SIO_INTR_FLAG | V_BLANK_INTR_FLAG;
    *(vu16 *)REG_STAT = STAT_V_BLANK_IF_ENABLE;
    *(vu16 *)REG_IME = 1;

    RfuSetUnk04(8);
    MainMenuFadeOut = FALSE;
    Lang = 0;

    MainMenuInit();
    Proc = MainMenu;

    for (;;) {
        VBlankIntrWait();
        KeyRead();
        Proc();
    }
}
