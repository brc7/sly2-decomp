/* EE kernel syscall stub -95 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iDisableDmacHandler\n"
    ".ent iDisableDmacHandler\n"
    "iDisableDmacHandler:\n"
    "addiu $3, $0, -95\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iDisableDmacHandler\n"
    ".set reorder\n"
);
