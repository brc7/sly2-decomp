/* EE kernel syscall stub -94 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iEnableDmacHandler\n"
    ".ent iEnableDmacHandler\n"
    "iEnableDmacHandler:\n"
    "addiu $3, $0, -94\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iEnableDmacHandler\n"
    ".set reorder\n"
);
