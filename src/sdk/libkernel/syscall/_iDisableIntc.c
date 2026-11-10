/* EE kernel syscall stub -27 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iDisableIntc\n"
    ".ent _iDisableIntc\n"
    "_iDisableIntc:\n"
    "addiu $3, $0, -27\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iDisableIntc\n"
    ".set reorder\n"
);
