@********************************************************************
@*          crt0.s                                                  *
@*            Start-up Routine (GAS)                                *
@*                                                                  *
@*          Copyright (C) 1999-2001 NINTENDO Co.,Ltd.               *
@*                                                                  *
@*          Ver 2002/02/15                                          *
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
@-                          Reset                                   -
@--------------------------------------------------------------------
    .EXTERN     AgbMain
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
        ldr     r1, =AgbMain            @ Starts and switches to 16-bit code
        mov     lr, pc
        bx      r1
        b       start_vector            @ Resets

    .ALIGN
sp_usr: .word   WRAM_END - 0x100
sp_irq: .word   WRAM_END - 0x60


@--------------------------------------------------------------------
@-  Interrupt Branch Processing (Multiple Interrupts) (Look-up Table) 42-80c -
@--------------------------------------------------------------------
    .EXTERN     IntrTable
    .EXTERN     STWI_status

    .GLOBAL     intr_main
    .ALIGN
    .CODE 32
intr_main:
        mov     r3, #REG_BASE           @ Reads IE/IF
        add     r3, r3, #OFFSET_REG_IE  @ r3:  REG_IE
        ldr     r12, [r3]               @ r12: IF|IE

        mrs     r0, spsr                @ Saves register (IRQ mode)
        stmfd   sp!, {r0, r3, r12, lr}  @ {spsr, REG_IE, IF|IE, lr}

        mov     r0, #1                  @ IME = 1 (To permit multiple interrupts
                                        @          even when an interrupt occurs while executing IME=0)
        strh    r0, [r3, #REG_IME - REG_IE]

        and     r1, r12, r12, lsr #16   @ Checks IE/IF
        mov     r2, #0

        ands    r0, r1, #SIO_INTR_FLAG
        bne     jump_intr                    @ Serial communication interrupt
        add     r2, r2, #4

        ands    r0, r1, #V_BLANK_INTR_FLAG   @ V-Blank interrupt
        bne     jump_intr
        add     r2, r2, #4

jump_intr:
        strh    r0, [r3, #2]                    @ Clears IF         23c
                                                @ Sets IE <- Selects multiple interrupts

        mov     r1, #SIO_INTR_FLAG
        and     r1, r1, r12
        strh    r1, [r3]

        mrs     r3, cpsr                            @ Permits multiple interrupts and switches to system mode
        bic     r3, r3, #PSR_CPU_MODE_MASK | PSR_IRQ_DISABLE | PSR_FIQ_DISABLE
        orr     r3, r3, #PSR_SYS_MODE
        msr     cpsr, r3
@---------------------------------------------------------------------System mode
        ldr     r1, =IntrTable                      @ Jumps to user IRQ process
        add     r1, r1, r2
        ldr     r0, [r1]

        stmfd   sp!, {lr}                           @ Saves register (System mode)
        adr     lr, intr_return                     @ Sets return address
        bx      r0
intr_return:
        ldmfd   sp!, {lr}                           @ Returns register (System mode)

        mrs     r3, cpsr                            @ Disables multiple interrupts and switches to IRQ mode
        bic     r3, r3, #PSR_CPU_MODE_MASK | PSR_IRQ_DISABLE | PSR_FIQ_DISABLE
        orr     r3, r3, #PSR_IRQ_MODE      | PSR_IRQ_DISABLE
        msr     cpsr, r3
@---------------------------------------------------------------------IRQ mode
        ldmfd   sp!, {r0, r3, r12, lr}          @ Returns register (IRQ mode)
        strh    r12, [r3]                       @ {spsr, REG_IE, IF|IE, lr}
        msr     spsr, r0
        bx      lr                              @ Returns prior to interrupt


@   .ORG    0x200

    .END

