@********************************************************************
@*          crt0.s                                                  *
@*            Startup Routine (GAS)		                    *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@********************************************************************
    .INCLUDE    "AgbDefine.s"
    .INCLUDE    "AgbMemoryMap.s"
    .INCLUDE    "AgbSyscallDefine.s"
    .INCLUDE    "AgbMacro.s"
    .TEXT

    .GLOBAL     _start
_start:
    .INCLUDE    "rom_header.s"

@--------------------------------------------------------------------
@-                          Expand Program                          -
@--------------------------------------------------------------------
    .EXTERN     _binary_demo_LZ_bin_start
    .GLOBAL     start_vector
    .CODE 32
start_vector:
        ldr     r0, =_binary_demo_LZ_bin_start
        ldr     r1, =CPU_WRAM + 0x100
        swi     SWI_NO_LZ77_UNCOMP_WRAM << 16   @ bl  LZ77UnCompWram

        ldr     lr, =CPU_WRAM + 0x100
        bx      lr


	.END


