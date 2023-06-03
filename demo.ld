SECTIONS {
    . = 0x03000100;
    .text : { *(.text) *(.rodata) }
    .bss : { *(.bss) }

    LZ_460c = 0x0300460c;
    LZ_43c8 = 0x030043c8;
    LZ_4420 = 0x03004420;
    u8_03005764 = 0x03005764;
    Lang = 0x03005758;
    Proc = 0x03005c90;
    IntrTableBuf = 0x030043c0;
    LangMsg = 0x0300575c;
    u8_03005754 = 0x03005754;
    GameList = 0x030056a0;
}
