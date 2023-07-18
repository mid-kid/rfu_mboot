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
        rfu_REQ_configGameData.o(.text)
        rfu_REQ_configSystem.o(.text)
        rfu_REQ_startSearchParent.o(.text)
        rfu_REQ_pollSearchParent.o(.text)
        rfu_REQ_endSearchParent.o(.text)
        rfu_STC_readParentCandidateList.o(.text)
        rfu_REQ_startConnectParent.o(.text)
        rfu_REQ_pollConnectParent.o(.text)
        rfu_REQ_endConnectParent.o(.text)
        rfu_getConnectParentStatus.o(.text)
        RfuSetUnk04.o(.text)
        rfu_syncVBlank.o(.text)
        rfu_REQBN_watchLink.o(.text)
        rfu_REQ_disconnect.o(.text)
        rfu_STC_fastCopy.o(.text)
        rfu_STC_fastCopy_end = .|1;
        Sio32IntrProcSet.o(.text)
        rfu_REQ_changeMasterSlave.o(.text)
        rfu_REQ_changeMasterSlave_force.o(.text)
        rfu_getMasterSlave.o(.text)
        rfu_clearAllSlot.o(.text)
        rfu_STC_releaseFrame.o(.text)
        rfu_clearSlot.o(.text)
        RfuMbootCfg.o(.text)
        MbootDLStart1.o(.text)
        MbootDLStart2.o(.text)
        RfuMbootDLStart.o(.text)
        rfu_REQ_sendData.o(.text)
        rfu_constructSendLLFrame.o(.text)
        rfu_STC_NI_constructLLSF.o(.text)
        rfu_REQ_recvData.o(.text)
        rfu_STC_CHILD_analyzeRecvPacket.o(.text)
        rfu_STC_analyzeLLSF.o(.text)
        rfu_STC_NI_receive_Sender.o(.text)
        rfu_STC_NI_receive_Receiver.o(.text)
        rfu_STC_NI_initSlot_asRecvControllData.o(.text)
        rfu_STC_NI_initSlot_asRecvDataEntity.o(.text)
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
