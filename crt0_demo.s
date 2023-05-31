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
        strh    r0, [r3, #8]                 @ Set REG_IME

        and     r1, r12, r12, lsr #16        @ r1:  IE & IF
        mov     r2, #0                       @ Checks IE/IF

        ands    r0, r1, #SIO_INTR_FLAG       @ Serial communication interrupt
        bne     jump_intr
        add     r2, r2, #4

        ands    r0, r1, #V_BLANK_INTR_FLAG   @ V-blank interrupt
        bne     jump_intr
        add     r2, r2, #4

jump_intr:
        strh    r0, [r3, #2]            @ Clears IF           11c

        mov     r1, #SIO_INTR_FLAG
        and     r1, r1, r12
        strh    r1, [r3]

        mrs     r3, cpsr
        bic     r3, r3, #(PSR_IRQ_FIQ_DISABLE | PSR_CPU_MODE_MASK)
        orr     r3, r3, #PSR_SYS_MODE
        msr     cpsr_all, r3

        ldr     r1, =IntrTableBuf       @ Jumps to user IRQ process
        add     r1, r1, r2
        ldr     r0, [r1]
        stmfd   sp!, {lr}
        adr     lr, 1f
        bx      r0
1:
        ldmfd   sp!, {lr}

        mrs     r3, cpsr
        bic     r3, r3, #(PSR_IRQ_FIQ_DISABLE | PSR_CPU_MODE_MASK)
        orr     r3, r3, #(PSR_IRQ_DISABLE | PSR_IRQ_MODE)
        msr     cpsr_all, r3

        ldmia   sp!, {r0, r3, r12, lr}
        strh    r12, [r3]
        msr     spsr_all, r0
        bx      lr


    .END

