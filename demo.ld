SECTIONS {
    . = 0x03000100;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }

    bss_start = .;
    .bss : {
        bss_Menu.o(.bss)
        bss_Game.o(COMMON)
        bss_Game.o(.bss)
        myFunc.o(.bss)
        bss_4.o(.bss)
        bss_Sio32.o(.bss)

        /* COMMON ordering shenanigans */
        . = ALIGN(0x10);
        common = .;
        bss_5.o(COMMON)
        SearchMenuErrorTimer = common; common += 4;
        MenuBusy = common; common += 4;
        Lang = common; common += 4;
        MenuMsg = common; common += 4;
        my_state = common; common += 4;
        MainMenuFadeOut = common; common += 4;

        bss_6.o(COMMON)
        sound.o(COMMON)
        bss_8.o(COMMON)
        myFunc.o(COMMON)
        bss_9.o(COMMON)
        bss_Rfu.o(COMMON)
        bss_11.o(.bss)
        bss_11.o(COMMON)
        bss_12.o(COMMON)
    }
    bss_end = .;
}
