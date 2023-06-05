SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        dump/baserom_demo_02a0.o(.text)
        dump/baserom_demo_03bc.o(.text)
        MenuMsgInit.o(.text)
        dump/baserom_demo_04c4.o(.text)
        GameListInit.o(.text)
        dump/baserom_demo_1190.o(.text)
        MenuMsgSet.o(.text)
        FrameCountReset.o(.text)
        MenuMsgBlink.o(.text)
        GameNameInit.o(.text)
        KeyRead.o(.text)
        dump/baserom_demo_1410.o(.text)
        BgScClearGame.o(.text)
        VramDrawBg2_MainMenu.o(.text)
        VramDrawBg2_SearchMenu.o(.text)
        VramClearBg2.o(.text)
        BgScSet.o(.text)
        BgScClear.o(.text)
        Delay3s.o(.text)
        WinInit.o(.text)
        WinFade.o(.text)
        SoundInit.o(.text)
        SoundMain.o(.text)
        SoundPlaySfx.o(.text)
        SoundStop.o(.text)
        dump/baserom_demo_1768.o(.text)
        Sio32Init.o(.text)
        dump/baserom_demo_26d0.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysLZ77UnCompVram.o(.text)
        dump/libagbsyscall/AgbSysRegisterRamReset.o(.text)
        dump/libagbsyscall/AgbSysSoftReset.o(.text)
        dump/libagbsyscall/AgbSysVBlankIntrWait.o(.text)
        dump/libgcc/_call_via_rX.o(.text)

        dump/baserom_demo_3f38.o(.rodata)
        MenuMsgInitial.o(.rodata)
        dump/baserom_demo_4200.o(.rodata)
    }
    .bss : { }

    /* Discard anything not explicitly listed */
    /DISCARD/ : { *(*) }

    LZ_460c = 0x0300460c;
    LZ_43c8 = 0x030043c8;
    LZ_4420 = 0x03004420;
    u8_03005764 = 0x03005764;
    Lang = 0x03005758;
    Proc = 0x03005c90;
    IntrTableBuf = 0x030043c0;
    MenuMsg = 0x0300575c;
    u8_03005754 = 0x03005754;
    GameList = 0x030056a0;
    BgBak = 0x03005790;
    Keys = 0x03005c94;
    Sound = 0x03005780;
    SoundSfxTable = 0x030043a4;
    Sio32 = 0x03005738;
    FrameCount = 0x03005698;
    GameName = 0x03005770;
    GameNameInitial = 0x03003f70;
}
