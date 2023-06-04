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
    dd if=baserom_demo.bin bs=1 skip=$skip count=$size 2>/dev/null | \
        ./dump_bin2c.py $1 $3 > dump/baserom_demo_$(printf "%04x" $off).c
    off=$(expr $off + $size)
}

split .text 0x02a0
split .text 0x03bc DemoMain
split .text 0x04a8
split .text 0x04c4 LangMsgInit
split .text 0x1158
split .text 0x1190 GameListInit
split .text 0x13f0
split .text 0x1410 KeyRead
split .text 0x1464
split .text 0x148c BgScClearGame
split .text 0x14f0 VramDrawBg2_MainMenu
split .text 0x1528 VramDrawBg2_SearchMenu
split .text 0x1548 VramClearBg2
split .text 0x157c BgScSet
split .text 0x15c4 BgScClear
split .text 0x15dc Delay3s
split .text 0x1614 WinInit
split .text 0x1670 WinFade
split .text 0x3ed0 # Library start
split .text 0x3f38
split .rodata 0x3fb8
split .rodata 0x4200 LangMsgInitial
split .rodata 0x5684
