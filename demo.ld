SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        Main.o(.text)

        searchMenu.o(.text)
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
        checkAPI_Error.o(.text)
        SearchMenuErrorBeep.o(.text)

        MenuMsgSet.o(.text)
        FrameCountReset.o(.text)
        MenuMsgBlink.o(.text)
        GameNameInit.o(.text)
        myFunc.o(.text)
        WinInit.o(.text)
        WinFade.o(.text)

        SoundInit.o(.text)
        SoundMain.o(.text)
        SoundPlaySfx.o(.text)
        SoundStop.o(.text)

        VBlankIntr.o(.text)
        Sio32.o(.text)

        AgbRFU_STWI.o(.text)

        RfuResetAdapter.o(.text)
        RfuBoot.o(.text)

        AgbRFU_Sio32ID.o(.text)

        rfu_REQ_RFUStatus.o(.text)
        rfu_initializeAPI.o(.text)
        rfu_STC_clearAPIVariables.o(.text)
        rfu_REQ_reset.o(.text)
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
        rfu_STC_fastCopy.o(.text)
        RfuMemcpyEnd = .|1;
        Sio32IntrProcSet.o(.text)
        RfuWaitDataStart.o(.text)
        RfuWaitDataStartForce.o(.text)
        RfuWaitDataDone.o(.text)
        rfu_clearAllSlot.o(.text)
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
        *(.text)

        SearchProcTable.o(.rodata)
        GameNameInit.o(.rodata)
        UserNames.o(.rodata)
        MenuMsgInitial.o(.rodata)
        Main.o(.rodata)
        Sfx.o(.rodata)
        interrupt.o(.rodata)
        char/pals_LZ.o(.rodata)
        char/tmap_LZ.o(.rodata)
        LZ_450c = .;
        char/search_tmap_LZ.o(.rodata)
        char/tiles_LZ.o(.rodata)
        AgbRFU_Sio32ID.o(.rodata)
        Sio32ID.o(.rodata)
        RfuEncTable.o(.rodata)
        RfuID.o(.rodata)
        *(.data)
    }

    bss_start = .;
    .bss : {
        bss_Menu.o(.bss)
        bss_Game.o(COMMON)
        bss_Game.o(.bss)
        myFunc.o(.bss)
        bss_4.o(.bss)
        bss_Sio32.o(.bss)

        /* COMMON ordering shenanigans */
        . = ALIGN(0x10);
        common = .;
        bss_5.o(COMMON)
        SearchMenuErrorTimer = common; common += 4;
        MenuBusy = common; common += 4;
        Lang = common; common += 4;
        MenuMsg = common; common += 4;
        my_state = common; common += 4;
        MainMenuFadeOut = common; common += 4;

        bss_6.o(COMMON)
        bss_Sound.o(.bss)
        bss_8.o(COMMON)
        bss_9.o(COMMON)
        bss_Rfu.o(COMMON)
        bss_11.o(.bss)
        bss_11.o(COMMON)
        bss_12.o(COMMON)
    }
    bss_end = .;
}
