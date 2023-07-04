#!/bin/sh
set -e
dd if=baserom.bin bs=1 skip=672 2>/dev/null | ./dump_lz77.py > baserom_demo.bin

rm -rf dump
mkdir -p dump

# Dump libraries
mkdir -p dump/libagbsyscall
( cd dump/libagbsyscall
    thumb-elf-ar x "$AGBDIR/lib/libagbsyscall.a"
)
mkdir -p dump/libgcc
( cd dump/libgcc
    thumb-elf-ar x "$(thumb-elf-gcc -print-libgcc-file-name)"
)

off=256
split() {
    skip=$(expr $off - 256 || true)
    size=$(expr $(printf %d $2) - $off)
    name="$3"
    if [ -z "$name" ]; then
        name="baserom_demo_$(printf "%04x" $off)"
    fi
    dd if=baserom_demo.bin bs=1 skip=$skip count=$size 2>/dev/null | \
        ./dump_bin2c.py $1 $3 > dump/"$name.c"
    off=$(expr $off + $size)
}

split .text 0x02a0
split .text 0x03bc DemoMain
split .text 0x04a8 MainMenuInit
split .text 0x04c4 MenuMsgInit
split .text 0x0534 MainMenu
split .text 0x0654 SearchMenuInit
split .text 0x06c0 SearchMenuDrawListTitle
split .text 0x0c08 SearchMenu
split .text 0x0c90 SearchMenuMbootStart
split .text 0x0e10 SearchMenuMbootDL
split .text 0x0eb0 RfuIntrDataTransfer
split .text 0x0ed0 RfuStrcmp
split .text 0x0efc SearchProc_GameInfoSet
split .text 0x0f10 SearchProc_RadioCfg
split .text 0x0f28 SearchProc_Connect
split .text 0x0f5c SearchProc_ConnectCheck
split .text 0x0f6c SearchProc_WaitData
split .text 0x0f84 RfuWaitData
split .text 0x0fa0 SearchProc_Boot
split .text 0x1118 SearchMenuUpdateGames
split .text 0x1158 SearchMenuClearGame
split .text 0x1190 GameListInit
split .text 0x1204 SearchMenuDrawList
split .text 0x124c SearchMenuDrawGame
split .text 0x1280 SearchMenuErrorCheck
split .text 0x12b8 SearchMenuErrorBeep
split .text 0x1310 MenuMsgSet
split .text 0x131c FrameCountReset
split .text 0x1370 MenuMsgBlink
split .text 0x13f0 GameNameInit
split .text 0x1410 KeyRead
split .text 0x1464 KeyRepeatHold
split .text 0x148c BgScClearGame
split .text 0x14f0 VramDrawBg2_MainMenu
split .text 0x1528 VramDrawBg2_SearchMenu
split .text 0x1548 VramClearBg2
split .text 0x157c BgScSet
split .text 0x15c4 BgScClear
split .text 0x15dc Delay3s
split .text 0x1614 WinInit
split .text 0x1670 WinFade
split .text 0x16a0 SoundInit
split .text 0x1728 SoundMain
split .text 0x1758 SoundPlaySfx
split .text 0x1768 SoundStop
split .text 0x17a0 VBlankIntr
split .text 0x180c Sio32Intr
split .text 0x1b20 Sio32_Unk1
split .text 0x1e20 Sio32_Unk2
split .text 0x1ee0 Sio32WaitSIState
split .text 0x1f20 RfuCmdInit
split .text 0x1f58
split .text 0x2020 RfuCmdSend
split .text 0x2060 RfuCmdRecv
split .text 0x20c8
split .text 0x210c RfuCmd_Start
split .text 0x2150 RfuCmd_Status
split .text 0x2194 RfuCmd_0x13
split .text 0x221c RfuCmd_GameInfoSet
split .text 0x2278 RfuCmd_RadioCfg
split .text 0x22bc RfuCmd_DiscoveryStart
split .text 0x2300 RfuCmd_GameInfoGet
split .text 0x2344 RfuCmd_DiscoveryResults
split .text 0x2390 RfuCmd_Connect
split .text 0x23d4 RfuCmd_ConnectCheck
split .text 0x2418 RfuCmd_ConnectReady
split .text 0x2480 RfuCmd_DataSend
split .text 0x24c4 RfuCmd_DataRecv
split .text 0x2508 RfuCmd_WaitData
split .text 0x2554 RfuCmd_0x30
split .text 0x2598 RfuCmd_End
split .text 0x25ec RfuResetAdapter
split .text 0x25fc RfuBoot
split .text 0x2664 RfuHandshake
split .text 0x26d0 Sio32Init
split .text 0x27a0 Sio32InitUpdate
split .text 0x286c RfuIntrHandshake
split .text 0x2894
split .text 0x292c RfuInit
split .text 0x2984
split .text 0x29a0 RfuStart
split .text 0x2a40 RfuGameInfoSet
split .text 0x2a60 RfuRadioCfg
split .text 0x2a70 RfuDiscoveryStart
split .text 0x2a8c RfuGameInfoGet
split .text 0x2aa8 RfuDiscoveryResults
split .text 0x2b70 RfuCmd_GameInfoGet_Parse
split .text 0x2bc4 RfuConnect
split .text 0x2d08 RfuConnectCheck
split .text 0x2d18 RfuConnectReady
split .text 0x2d34 RfuCmd_ConnectCheck_Parse
split .text 0x2d4c
split .text 0x2d74
split .text 0x2eb0
split .text 0x2f28
split .text 0x2f58
split .text 0x2f64 Sio32IntrProcSet
split .text 0x2fb0 RfuWaitDataStart
split .text 0x2fd4 RfuWaitDataStartForce
split .text 0x2fe0 RfuWaitDataDone
split .text 0x3044 MbootInit
split .text 0x3078
split .text 0x3184
split .text 0x31c8 RfuMbootCfg
split .text 0x31f4
split .text 0x3224 MbootDLStart
split .text 0x33a8 RfuMbootDLStart
split .text 0x3454 RfuDataSend
split .text 0x353c RfuDataSendPrepare
split .text 0x36ec RfuDataSendPreparePeer
split .text 0x37b8 RfuDataRecv
split .text 0x3808 RfuCmd_DataRecv_Parse
split .text 0x397c RfuDataRecvParse
split .text 0x3b60
split .text 0x3ce0
split .text 0x3d38
split .text 0x3e04
split .text 0x3e44 RfuReset
split .text 0x3ed0 RfuResetSub
split .text 0x3f38  # Libraries
split .rodata 0x3f70 SearchProcTable
split .rodata 0x3f7c GameNameInitial
split .rodata 0x3fb8
split .rodata 0x4200 MenuMsgInitial
split .rodata 0x43c0
split .rodata 0x43c8 IntrTableBuf
split .rodata 0x5684
