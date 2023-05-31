@********************************************************************
@*          crt0_demo.s                                             *
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
    .INCLUDE    "rom_header_demo.s"

@--------------------------------------------------------------------
@-                          Reset                                -
@--------------------------------------------------------------------
    .EXTERN     DemoMain
    .GLOBAL     start_vector
    .CODE 32
start_vector:
        mov     r0, #PSR_IRQ_MODE       @ Switches to IRQ mode
        msr     cpsr, r0
        ldr     sp, sp_irq              @ Sets SP_irq
        mov     r0, #PSR_SYS_MODE       @ Switches to system mode
        msr     cpsr, r0
        ldr     sp, sp_usr              @ Sets SP_usr
        ldr     r1, =INTR_VECTOR_BUF    @ Sets interrupt address
        adr     r0, intr_main
        str     r0, [r1]
        ldr     r1, =DemoMain           @ Starts and switches to 16-bit code
        mov     lr, pc
        bx      r1
        b       start_vector            @ Resets

    .ALIGN
sp_usr: .word   WRAM_END - 0x100
sp_irq: .word   WRAM_END - 0x60


@--------------------------------------------------------------------
@-          Interrupt Branch Process (Jump Table) 32 bits    27-62c -
@--------------------------------------------------------------------
    .EXTERN     IntrTableBuf
    .GLOBAL     intr_main
    .ALIGN
    .CODE 32
intr_main:
        mov     r3, #REG_BASE            @ r12: REG_BASE
        add     r3, r3, #OFFSET_REG_IE   @ r3:  REG_IE
        ldr     r12, [r3]
        mrs     r0, spsr
        stmdb   sp!, {r0, r3, r12, lr}
        mov     r0, #1
        strh    r0, [r3, #8]

        and     r1, r12, r12, lsr #16
        mov     r2, #0
        ands    r0, r1, #128
        bne     intr_main_138
        add     r2, r2, #4
        ands    r0, r1, #1
        bne     intr_main_138
        add     r2, r2, #4
intr_main_138:
        strh    r0, [r3, #2]
        mov     r1, #128
        and     r1, r1, r12
        strh    r1, [r3]
        mrs     r3, cpsr
        bic     r3, r3, #223
        orr     r3, r3, #31
        msr     cpsr_all, r3
        ldr     r1, =0x030043c0
        add     r1, r1, r2
        ldr     r0, [r1]
        stmfd   sp!, {lr}
        adr     lr, intr_main_170
        bx      r0
intr_main_170:
        ldmfd   sp!, {lr}
        mrs     r3, cpsr
        bic     r3, r3, #223
        orr     r3, r3, #146
        msr     cpsr_all, r3
        ldmia   sp!, {r0, r3, r12, lr}
        strh    r12, [r3]
        msr     spsr_all, r0
        bx      lr

@        and     r1, r12, r12, lsr #16        @ r1:  IE & IF
@        mov     r2, #0                       @ Checks IE/IF
@        ands    r0, r1, #CASSETTE_INTR_FLAG  @ Game Pak interrupt
@loop:   bne     loop
@
@        ands    r0, r1, #SIO_INTR_FLAG | MULTI_SIO_TIMER_INTR_FLAG
@        bne     jump_intr                    @ Serial communication interrupt
@        add     r2, r2, #4                   @ or Multi-player communication timer interrupt
@
@        ands    r0, r1, #V_BLANK_INTR_FLAG   @ V-blank interrupt
@        strneh  r0, [r12, #INTR_CHECK_BUF - WRAM_END] @ Sets interrupt check
@        bne     jump_intr
@        add     r2, r2, #4
@
@        ands    r0, r1, #H_BLANK_INTR_FLAG   @ H-blank interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #V_COUNT_INTR_FLAG   @ V-counter interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #DMA0_INTR_FLAG      @ DMA 0 interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #DMA1_INTR_FLAG      @ DMA 1 interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #DMA2_INTR_FLAG      @ DMA 2 interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #DMA3_INTR_FLAG      @ DMA 3 interrupt
@        bne     jump_intr
@        add     r2, r2, #4
@        ands    r0, r1, #KEY_INTR_FLAG       @ Key interrupt
@jump_intr:
@        strh    r0, [r3, #2]            @ Clears IF           11c
@        ldr     r1, =IntrTableBuf       @ Jumps to user IRQ process
@        add     r1, r1, r2
@        ldr     r0, [r1]
@        bx      r0


    .END

