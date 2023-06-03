#!/bin/sh
set -e
dd if=baserom.bin bs=1 skip=672 2>/dev/null | ./dump_lz77.py > baserom_demo.bin

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
        ./dump_bin2c.py $1 > dump/baserom_demo_$(printf "%04x" $off).c
    off=$(expr $off + $size)
}

split .text 0x02a0  # DemoMain
split .text 0x03bc
split .text 0x04a8  # LangMsgInit
split .text 0x04c4
split .text 0x1158  # GameListInit
split .text 0x1190
split .text 0x3ed0  # Library start
split .text 0x3f38
split .rodata 0x3fb8  # LangMsgInitial
split .rodata 0x4200
split .rodata 0x5684
