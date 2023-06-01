SECTIONS {
    . = 0x03000100;
    .text : { *(.text) }
    .bss : { *(.bss) }
}
