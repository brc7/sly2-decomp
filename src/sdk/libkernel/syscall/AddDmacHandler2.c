/* EE kernel syscall stub 18 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl AddDmacHandler2\n"
    ".ent AddDmacHandler2\n"
    "AddDmacHandler2:\n"
    "addiu $3, $0, 18\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end AddDmacHandler2\n"
    ".set reorder\n"
);
