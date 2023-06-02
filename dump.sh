#!/bin/sh
set -e
dd if=baserom.bin bs=1 skip=672 2>/dev/null | ./dump_lz77.py > baserom_demo.bin

off=256
split() {
    skip=$(expr $off - 256 || true)
    size=$(expr $(printf %d $2) - $off)
    dd if=baserom_demo.bin bs=1 skip=$skip count=$size 2>/dev/null | \
        ./dump_bin2c.py $1 > baserom_demo_$(printf "%04x" $off).dump.c
    off=$(expr $off + $size)
}

split .text 0x2a0
split .text 0x3fb8
split .rodata 0x4200
split .rodata 0x5684
