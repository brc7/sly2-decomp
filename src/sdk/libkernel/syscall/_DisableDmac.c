/* EE kernel syscall stub 23 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _DisableDmac\n"
    ".ent _DisableDmac\n"
    "_DisableDmac:\n"
    "addiu $3, $0, 23\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _DisableDmac\n"
    ".set reorder\n"
);
