/* EE kernel syscall stub -26 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl _iEnableIntc\n"
    ".ent _iEnableIntc\n"
    "_iEnableIntc:\n"
    "addiu $3, $0, -26\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end _iEnableIntc\n"
    ".set reorder\n"
);
