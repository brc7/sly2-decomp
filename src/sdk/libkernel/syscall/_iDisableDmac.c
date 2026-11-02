/* EE kernel syscall stub -29 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iDisableDmac\n"
    ".ent _iDisableDmac\n"
    "_iDisableDmac:\n"
    "addiu $3, $0, -29\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iDisableDmac\n"
    ".set reorder\n"
);
