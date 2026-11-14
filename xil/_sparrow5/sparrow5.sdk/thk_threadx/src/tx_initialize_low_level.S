/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/

    .syntax unified
    .arm

    .global _tx_initialize_low_level
    .global _tx_thread_system_stack_ptr
    .global _tx_initialize_unused_memory
    .global __tx_irq_handler
    .global __tx_irq_processing_return
    .global __tx_fiq_handler
    .global __tx_fiq_processing_return

    .extern _tx_thread_context_save
    .extern _tx_thread_context_restore
    .extern _tx_thread_fiq_context_save
    .extern _tx_thread_fiq_context_restore
    .extern _tx_timer_interrupt
    .extern tx_zynq_tick_init
    .extern tx_zynq_irq_ack
    .extern tx_zynq_irq_eoi
    .extern tx_zynq_timer_clear
    .extern _end

IRQ_MODE = 0xD2

    .type _tx_initialize_low_level, function
_tx_initialize_low_level:
    MRS     r4, CPSR
    MOV     r5, lr
    MOV     r0, #IRQ_MODE
    MSR     CPSR_c, r0
    MOV     r1, sp
    LDR     r2, =_tx_thread_system_stack_ptr
    STR     r1, [r2]
    MSR     CPSR_c, r4
    MOV     lr, r5

    LDR     r1, =_end
    LDR     r2, =_tx_initialize_unused_memory
    ADD     r1, r1, #8
    STR     r1, [r2]

    PUSH    {lr}
    BL      tx_zynq_tick_init
    POP     {lr}
    BX      lr

    .type __tx_irq_handler, function
__tx_irq_handler:
    B       _tx_thread_context_save
__tx_irq_processing_return:
    PUSH    {r4, r5}
    BL      tx_zynq_irq_ack
    MOV     r4, r0
    CMP     r0, #29
    BNE     tx_irq_not_tick
    BL      tx_zynq_timer_clear
    BL      _tx_timer_interrupt
tx_irq_not_tick:
    MOV     r0, r4
    BL      tx_zynq_irq_eoi
    POP     {r4, r5}
    B       _tx_thread_context_restore

    .type __tx_fiq_handler, function
__tx_fiq_handler:
    B       _tx_thread_fiq_context_save
__tx_fiq_processing_return:
    B       _tx_thread_fiq_context_restore
