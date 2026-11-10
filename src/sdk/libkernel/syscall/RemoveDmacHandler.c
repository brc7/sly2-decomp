/* EE kernel syscall stub 19 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RemoveDmacHandler\n"
    ".ent RemoveDmacHandler\n"
    "RemoveDmacHandler:\n"
    "addiu $3, $0, 19\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RemoveDmacHandler\n"
    ".set reorder\n"
);
