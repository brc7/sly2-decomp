/* EE kernel syscall stub 12 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl Interrupt2Iop\n"
    ".ent Interrupt2Iop\n"
    "Interrupt2Iop:\n"
    "addiu $3, $0, 12\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end Interrupt2Iop\n"
    ".set reorder\n"
);
