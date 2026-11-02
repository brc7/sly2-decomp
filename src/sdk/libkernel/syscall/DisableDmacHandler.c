/* EE kernel syscall stub 95 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DisableDmacHandler\n"
    ".ent DisableDmacHandler\n"
    "DisableDmacHandler:\n"
    "addiu $3, $0, 95\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DisableDmacHandler\n"
    ".set reorder\n"
);
