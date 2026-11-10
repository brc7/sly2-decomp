/* EE kernel syscall stub 22 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _EnableDmac\n"
    ".ent _EnableDmac\n"
    "_EnableDmac:\n"
    "addiu $3, $0, 22\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _EnableDmac\n"
    ".set reorder\n"
);
