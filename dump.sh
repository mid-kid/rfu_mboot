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
split .text 0x03bc
split .text 0x04a8 MainMenuInit
split .text 0x04c4 MenuMsgInit
split .text 0x0534 MainMenu
split .text 0x0654 SearchMenuInit
split .text 0x06c0 SearchMenuDrawListTitle
split .text 0x0c08 SearchMenu
split .text 0x0c90 SearchMenuMbootStart
split .text 0x0e10 SearchMenuMbootDL
split .text 0x0eb0
split .text 0x0ed0 RfuStrcmp
split .text 0x0efc
split .text 0x0f10
split .text 0x0f28
split .text 0x0f5c
split .text 0x0f6c
split .text 0x0f84 RfuWaitData
split .text 0x0fa0
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
split .text 0x17a0
split .text 0x180c
split .text 0x1b20
split .text 0x1e20
split .text 0x1ee0
split .text 0x1f20
split .text 0x1f58
split .text 0x2020
split .text 0x2060
split .text 0x20c8
split .text 0x210c
split .text 0x2150
split .text 0x2194
split .text 0x221c
split .text 0x2278
split .text 0x22bc
split .text 0x2300
split .text 0x2344
split .text 0x2390
split .text 0x23d4
split .text 0x2418
split .text 0x2480
split .text 0x24c4
split .text 0x2508
split .text 0x2554
split .text 0x2598
split .text 0x25ec
split .text 0x25fc
split .text 0x2664
split .text 0x26d0 Sio32Init
split .text 0x27a0 Sio32InitUpdate
split .text 0x286c
split .text 0x2894
split .text 0x292c
split .text 0x2984
split .text 0x29a0
split .text 0x2a40
split .text 0x2a60
split .text 0x2a70
split .text 0x2a8c
split .text 0x2aa8
split .text 0x2b70
split .text 0x2bc4
split .text 0x2d08
split .text 0x2d18
split .text 0x2d34
split .text 0x2d4c
split .text 0x2d74
split .text 0x2eb0
split .text 0x2f28
split .text 0x2f58
split .text 0x2f64
split .text 0x2fb0 RfuWaitDataStart
split .text 0x2fd4 RfuWaitDataStartForce
split .text 0x2fe0 RfuWaitDataDone
split .text 0x3044
split .text 0x3078
split .text 0x3184
split .text 0x31c8 RfuMbootCfg
split .text 0x31f4
split .text 0x3224 MbootDLStart
split .text 0x33a8 RfuMbootDLStart
split .text 0x3454
split .text 0x353c
split .text 0x36ec
split .text 0x37b8
split .text 0x3808
split .text 0x397c
split .text 0x3b60
split .text 0x3ce0
split .text 0x3d38
split .text 0x3e04
split .text 0x3e44
split .text 0x3ed0 # Library start
split .text 0x3f38
split .rodata 0x3f70
split .rodata 0x3f7c GameNameInitial
split .rodata 0x3fb8
split .rodata 0x4200 MenuMsgInitial
split .rodata 0x5684
