/* EE kernel syscall stub 13 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetVTLBRefillHandler\n"
    ".ent SetVTLBRefillHandler\n"
    "SetVTLBRefillHandler:\n"
    "addiu $3, $0, 13\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetVTLBRefillHandler\n"
    ".set reorder\n"
);
