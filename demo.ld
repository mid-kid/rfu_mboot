SECTIONS {
    . = 0x03000100;
    .text : { *(.text) *(.rodata) }
    .bss : { *(.bss) }
    DemoMain = 0x030002a1;
    IntrTableBuf = 0x030043c0;
}
