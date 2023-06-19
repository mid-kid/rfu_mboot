SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        dump/baserom_demo_02a0.o(.text)
        MainMenuInit.o(.text)
        MenuMsgInit.o(.text)
        MainMenu.o(.text)
        SearchMenuInit.o(.text)
        SearchMenuDrawListTitle.o(.text)
        SearchMenu.o(.text)
        dump/baserom_demo_0e10.o(.text)
        RfuStrcmp.o(.text)
        dump/baserom_demo_0ed0.o(.text)
        dump/baserom_demo_0efc.o(.text)
        dump/baserom_demo_0f10.o(.text)
        dump/baserom_demo_0f28.o(.text)
        dump/baserom_demo_0f5c.o(.text)
        RfuWaitData.o(.text)
        dump/baserom_demo_0f84.o(.text)
        dump/baserom_demo_0fa0.o(.text)
        SearchMenuClearGame.o(.text)
        GameListInit.o(.text)
        SearchMenuDrawList.o(.text)
        SearchMenuDrawGame.o(.text)
        SearchMenuErrorCheck.o(.text)
        SearchMenuErrorBeep.o(.text)
        MenuMsgSet.o(.text)
        FrameCountReset.o(.text)
        MenuMsgBlink.o(.text)
        GameNameInit.o(.text)
        KeyRead.o(.text)
        KeyRepeatHold.o(.text)
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
        dump/baserom_demo_17a0.o(.text)
        dump/baserom_demo_180c.o(.text)
        dump/baserom_demo_1b20.o(.text)
        dump/baserom_demo_1e20.o(.text)
        dump/baserom_demo_1ee0.o(.text)
        dump/baserom_demo_1f20.o(.text)
        dump/baserom_demo_1f58.o(.text)
        dump/baserom_demo_2020.o(.text)
        dump/baserom_demo_2060.o(.text)
        dump/baserom_demo_20c8.o(.text)
        dump/baserom_demo_210c.o(.text)
        dump/baserom_demo_2150.o(.text)
        dump/baserom_demo_2194.o(.text)
        dump/baserom_demo_221c.o(.text)
        dump/baserom_demo_2278.o(.text)
        dump/baserom_demo_22bc.o(.text)
        dump/baserom_demo_2300.o(.text)
        dump/baserom_demo_2344.o(.text)
        dump/baserom_demo_2390.o(.text)
        dump/baserom_demo_23d4.o(.text)
        dump/baserom_demo_2418.o(.text)
        dump/baserom_demo_2480.o(.text)
        dump/baserom_demo_24c4.o(.text)
        dump/baserom_demo_2508.o(.text)
        dump/baserom_demo_2554.o(.text)
        dump/baserom_demo_2598.o(.text)
        dump/baserom_demo_25ec.o(.text)
        dump/baserom_demo_25fc.o(.text)
        Sio32Init.o(.text)
        Sio32InitUpdate.o(.text)
        dump/baserom_demo_27a0.o(.text)
        dump/baserom_demo_286c.o(.text)
        dump/baserom_demo_2894.o(.text)
        dump/baserom_demo_292c.o(.text)
        dump/baserom_demo_2984.o(.text)
        dump/baserom_demo_29a0.o(.text)
        dump/baserom_demo_2a40.o(.text)
        dump/baserom_demo_2a60.o(.text)
        dump/baserom_demo_2a70.o(.text)
        dump/baserom_demo_2a8c.o(.text)
        dump/baserom_demo_2aa8.o(.text)
        dump/baserom_demo_2b70.o(.text)
        dump/baserom_demo_2bc4.o(.text)
        dump/baserom_demo_2d08.o(.text)
        dump/baserom_demo_2d18.o(.text)
        dump/baserom_demo_2d34.o(.text)
        dump/baserom_demo_2d4c.o(.text)
        dump/baserom_demo_2d74.o(.text)
        dump/baserom_demo_2eb0.o(.text)
        dump/baserom_demo_2f28.o(.text)
        dump/baserom_demo_2f58.o(.text)
        RfuWaitDataStart.o(.text)
        RfuWaitDataStartForce.o(.text)
        RfuWaitDataDone.o(.text)
        dump/baserom_demo_2fe0.o(.text)
        dump/baserom_demo_3044.o(.text)
        dump/baserom_demo_3078.o(.text)
        RfuMbootCfg.o(.text)
        dump/baserom_demo_31c8.o(.text)
        MbootDLStart.o(.text)
        RfuMbootDLStart.o(.text)
        dump/baserom_demo_33a8.o(.text)
        dump/baserom_demo_3454.o(.text)
        dump/baserom_demo_353c.o(.text)
        dump/baserom_demo_36ec.o(.text)
        dump/baserom_demo_37b8.o(.text)
        dump/baserom_demo_3808.o(.text)
        dump/baserom_demo_397c.o(.text)
        dump/baserom_demo_3b60.o(.text)
        dump/baserom_demo_3ce0.o(.text)
        dump/baserom_demo_3d38.o(.text)
        dump/baserom_demo_3e04.o(.text)
        dump/baserom_demo_3e44.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysLZ77UnCompVram.o(.text)
        dump/libagbsyscall/AgbSysRegisterRamReset.o(.text)
        dump/libagbsyscall/AgbSysSoftReset.o(.text)
        dump/libagbsyscall/AgbSysVBlankIntrWait.o(.text)
        dump/libgcc/_call_via_rX.o(.text)

        dump/baserom_demo_3f38.o(.rodata)
        GameNameInit.o(.rodata)
        dump/baserom_demo_3f7c.o(.rodata)
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
    KeyTimers = 0x03005724;
    MainMenuFadeOut = 0x03005764;
    StringEnglish = 0x03004200;
    MenuState = 0x03005760;
    MenuBusy = 0x03005754;
    SearchMenuCursor = 0x03005691;
    GameListBits = 0x03005721;
    SearchMenuErrorMsg = 0x03005693;
    GameLogoInitial = 0x03003f7c;
    Mboot = 0x030060c0;
    MbootBeaconID = 0x03005694;
    MbootPeer = 0x03005692;
    RfuBuf = 0x03006190;
    RfuPeers = 0x03005f00;
    SearchMenuEnd = 0x03005690;
    SearchMenuErrorTimer = 0x03005750;
    SearchMenuFoundGames = 0x03005720;
    SearchMenuTimer = 0x03005696;
    SearchProcTable = 0x03003f38;
    LZ_450c = 0x0300450c;
    Rfu = 0x03005ca0;
    struct_unk_6180 = 0x03006180;
}
