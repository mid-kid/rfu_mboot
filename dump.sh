#!/bin/sh
set -e
dd if=baserom.bin bs=1 skip=672 2>/dev/null | ./dump_lz77.py > baserom_demo.bin

off=416
split() {
    dd if=baserom_demo.bin bs=1 skip=$off count=$2 2>/dev/null | \
        ./dump_bin2c.py $1 > baserom_demo_$(printf "%04x" $off).dump.c
    off=$(expr $off + $2)
}

split .text 21476
