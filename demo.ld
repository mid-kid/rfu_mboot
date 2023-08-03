SECTIONS {
    . = 0x03000100;
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }

    /DISCARD/ : {
        searchMenu.o(COMMON)
    }

    bss_start = .;
    .bss : { *(.bss) }
    /* COMMON ordering shenanigans */
    .bss : {
        /* searchMenu.o(COMMON) */
        . = ALIGN(0x10);
        SearchMenuErrorTimer = .; . += 4;
        MenuBusy = .; . += 4;
        Lang = .; . += 4;
        MenuMsg = .; . += 4;
        my_state = .; . += 4;
        MainMenuFadeOut = .; . += 4;
        . = ALIGN(0x10);
        GameName = .; . += 16;

        *(COMMON)
    }
    bss_end = .;
}
