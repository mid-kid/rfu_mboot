SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        DemoMain.o(.text)
        MainMenuInit.o(.text)
        MenuMsgInit.o(.text)
        MainMenu.o(.text)
        SearchMenuInit.o(.text)
        SearchMenuDrawListTitle.o(.text)
        SearchMenu.o(.text)
        RfuIntrDataTransfer.o(.text)
        RfuStrcmp.o(.text)
        SearchProc_GameInfoSet.o(.text)
        SearchProc_RadioCfg.o(.text)
        SearchProc_Connect.o(.text)
        SearchProc_ConnectCheck.o(.text)
        SearchProc_WaitData.o(.text)
        RfuWaitData.o(.text)
        SearchProc_Boot.o(.text)
        SearchMenuUpdateGames.o(.text)
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
        VBlankIntr.o(.text)
        Sio32.o(.text)
        RfuCmdReset.o(.text)
        RfuCmdInit.o(.text)
        RfuCmdSend.o(.text)
        RfuCmdRecv.o(.text)
        Sio32VBlank.o(.text)
        RfuCmd_Start.o(.text)
        RfuCmd_Status.o(.text)
        RfuCmd_0x13.o(.text)
        RfuCmd_GameInfoSet.o(.text)
        RfuCmd_RadioCfg.o(.text)
        RfuCmd_DiscoveryStart.o(.text)
        RfuCmd_GameInfoGet.o(.text)
        RfuCmd_DiscoveryResults.o(.text)
        RfuCmd_Connect.o(.text)
        RfuCmd_ConnectCheck.o(.text)
        RfuCmd_ConnectReady.o(.text)
        RfuCmd_DataSend.o(.text)
        RfuCmd_DataRecv.o(.text)
        RfuCmd_WaitData.o(.text)
        RfuCmd_0x30.o(.text)
        RfuCmd_End.o(.text)
        RfuResetAdapter.o(.text)
        RfuBoot.o(.text)
        RfuHandshake.o(.text)
        Sio32Init.o(.text)
        Sio32InitUpdate.o(.text)
        RfuIntrHandshake.o(.text)
        Rfu0x13.o(.text)
        RfuInit.o(.text)
        MbootInit.o(.text)
        RfuStart.o(.text)
        RfuGameInfoSet.o(.text)
        RfuRadioCfg.o(.text)
        RfuDiscoveryStart.o(.text)
        RfuGameInfoGet.o(.text)
        RfuDiscoveryResults.o(.text)
        RfuCmd_GameInfoGet_Parse.o(.text)
        RfuConnect.o(.text)
        RfuConnectCheck.o(.text)
        RfuConnectReady.o(.text)
        RfuCmd_ConnectCheck_Parse.o(.text)
        RfuSetUnk04.o(.text)
        RfuVBlank.o(.text)
        RfuStatus.o(.text)
        RfuDisconnect.o(.text)
        RfuMemcpy.o(.text)
        RfuMemcpyEnd = .|1;
        Sio32IntrProcSet.o(.text)
        RfuWaitDataStart.o(.text)
        RfuWaitDataStartForce.o(.text)
        RfuWaitDataDone.o(.text)
        MbootReset.o(.text)
        RfuPeerUpdate.o(.text)
        RfuPeerUpdateFlags.o(.text)
        RfuMbootCfg.o(.text)
        MbootDLStart1.o(.text)
        MbootDLStart2.o(.text)
        RfuMbootDLStart.o(.text)
        RfuDataSend.o(.text)
        RfuDataSendPrepare.o(.text)
        RfuDataSendPreparePeer.o(.text)
        RfuDataRecv.o(.text)
        RfuCmd_DataRecv_Parse.o(.text)
        RfuDataRecvParse.o(.text)
        RfuDataRecvHandle1.o(.text)
        RfuDataRecvHandle2.o(.text)
        RfuDataRecvWrite1.o(.text)
        RfuDataRecvWrite2.o(.text)
        RfuReset.o(.text)
        RfuResetSub.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysLZ77UnCompVram.o(.text)
        dump/libagbsyscall/AgbSysRegisterRamReset.o(.text)
        dump/libagbsyscall/AgbSysSoftReset.o(.text)
        dump/libagbsyscall/AgbSysVBlankIntrWait.o(.text)
        dump/libgcc/_call_via_rX.o(.text)

        SearchProcTable.o(.rodata)
        GameNameInit.o(.rodata)
        UserNames.o(.rodata)
        MenuMsgInitial.o(.rodata)
        MainMenuInit.o(.rodata)
        Sfx.o(.rodata)
        IntrTableBuf.o(.rodata)
        dump/baserom_demo_43c8.o(.rodata)

        UserNames.o(.data)
    }
    .bss : { }

    LZ_43c8 = 0x030043c8;
    LZ_4420 = 0x03004420;
    LZ_450c = 0x0300450c;
    LZ_460c = 0x0300460c;
    HandshakeString = 0x03005630;
    RfuEncTable = 0x03005648;
    UserNames = 0x03005674;

    bss_start = 0x03005684;

    SearchMenuEnd = 0x03005690;
    SearchMenuCursor = 0x03005691;
    MbootPeer = 0x03005692;
    SearchMenuErrorMsg = 0x03005693;
    MbootBeaconID = 0x03005694;
    SearchMenuTimer = 0x03005696;
    FrameCount = 0x03005698;
    GameList = 0x030056a0;
    GameListBits = 0x03005720;
    GameListBitsNew = 0x03005721;
    KeyTimers = 0x03005724;
    u8_03005728 = 0x03005728;
    u8_03005729 = 0x03005729;
    RfuIntrSize = 0x0300572a;
    RfuIntrCmd = 0x03005730;
    Sio32 = 0x03005738;
    SearchMenuErrorTimer = 0x03005750;
    MenuBusy = 0x03005754;
    u8_03005754 = 0x03005754;
    Lang = 0x03005758;
    MenuMsg = 0x0300575c;
    MenuState = 0x03005760;
    MainMenuFadeOut = 0x03005764;
    u8_03005764 = 0x03005764;
    GameName = 0x03005770;
    Sound = 0x03005780;
    BgBak = 0x03005790;
    Proc = 0x03005c90;
    Keys = 0x03005c94;
    Sio32IntrProc = 0x03005c98;
    Rfu = 0x03005ca0;
    RfuBufSend = 0x03005cc0;
    RfuBufRecv = 0x03005de0;
    u8_03005efc = 0x03005efc;
    RfuPeers = 0x03005f00;
    Mboot = 0x030060c0;
    MbootTmp = 0x03006180;
    struct_unk_6180 = 0x03006180;
    RfuBuf = 0x03006190;
    RfuMemcpyFunc = 0x03006198;
    RfuDataSendBuf = 0x030061f8;

    bss_end = 0x03006310;

    /* Discard anything not explicitly listed */
    /DISCARD/ : { *(*) }
}
