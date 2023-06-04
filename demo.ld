SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        dump/baserom_demo_02a0.o(.text)
        dump/baserom_demo_03bc.o(.text)
        LangMsgInit.o(.text)
        dump/baserom_demo_04c4.o(.text)
        GameListInit.o(.text)
        dump/baserom_demo_1190.o(.text)
        BgScClear.o(.text)
        dump/baserom_demo_15c4.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysCpuSet.o(.text)
        dump/libagbsyscall/AgbSysLZ77UnCompVram.o(.text)
        dump/libagbsyscall/AgbSysRegisterRamReset.o(.text)
        dump/libagbsyscall/AgbSysSoftReset.o(.text)
        dump/libagbsyscall/AgbSysVBlankIntrWait.o(.text)
        dump/libgcc/_call_via_rX.o(.text)

        dump/baserom_demo_3f38.o(.rodata)
        LangMsgInitial.o(.rodata)
        dump/baserom_demo_4200.o(.rodata)
    }
    .bss : { }

    /* Discard anything not explicitly listed */
    /DISCARD/ : { *(*) }

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
    BgBak = 0x03005790;
}
