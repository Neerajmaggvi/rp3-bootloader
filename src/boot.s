.section ".text.boot"
.global _start

_start:
    /* ── 1. Read current exception level ── */
    mrs     x0, CurrentEL
    lsr     x0, x0, #2
    and     x0, x0, #3

    /* ── 2. If EL2, drop to EL1 ── */
    cmp     x0, #2
    bne     .el1_entry

    mov     x0, #(1 << 31)
    msr     hcr_el2, x0

    mov     x0, #0x3c5
    msr     spsr_el2, x0

    adr     x0, .el1_entry
    msr     elr_el2, x0
    eret

.el1_entry:
    /* ── 3. Park cores 1, 2, 3 ── */
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbnz    x1, .park

    /* ── 4. Set up stack ── */
    ldr     x1, =__stack_top
    mov     sp, x1

    /* ── 5. Zero BSS ── */
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    mov     x3, #0
.zero_bss:
    cmp     x1, x2
    bge     .bss_done
    str     x3, [x1], #8
    b       .zero_bss
.bss_done:

    /* ── 6. Jump to C ── */
    bl      main
    b       .park

.park:
    wfe
    b       .park

    