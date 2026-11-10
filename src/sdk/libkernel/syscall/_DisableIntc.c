/* EE kernel syscall stub 21 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _DisableIntc\n"
    ".ent _DisableIntc\n"
    "_DisableIntc:\n"
    "addiu $3, $0, 21\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _DisableIntc\n"
    ".set reorder\n"
);
