SECTIONS {
    . = 0x03000100;
    .text : {
        crt0_demo.o(.text)
        Main.o(.text)
        searchMenu.o(.text)
        menuFunc.o(.text)
        myFunc.o(.text)
        sound.o(.text)
        interrupt.o(.text)
        AgbRFU_sio32intr.o(.text)
        AgbRFU_STWI.o(.text)
        AgbRFU_Sio32ID.o(.text)
        AgbRFU_LL.o(.text)
        *(.text)

        searchMenu.o(.rodata)
        menuFunc.o(.rodata)
        UserNames.o(.rodata)
        MenuMsgInitial.o(.rodata)
        Main.o(.rodata)
        Sfx.o(.rodata)
        interrupt.o(.rodata)
        char/pals_LZ.o(.rodata)
        char/tmap_LZ.o(.rodata)
        LZ_450c = .;
        char/search_tmap_LZ.o(.rodata)
        char/tiles_LZ.o(.rodata)
        AgbRFU_Sio32ID.o(.rodata)
        AgbRFU_LL.o(.rodata)
        *(.data)
    }

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
        bss_9.o(COMMON)
        bss_Rfu.o(COMMON)
        bss_11.o(.bss)
        bss_11.o(COMMON)
        bss_12.o(COMMON)
    }
    bss_end = .;
}
