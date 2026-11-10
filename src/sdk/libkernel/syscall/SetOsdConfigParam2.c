/* EE kernel syscall stub 110 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetOsdConfigParam2\n"
    ".ent SetOsdConfigParam2\n"
    "SetOsdConfigParam2:\n"
    "addiu $3, $0, 110\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetOsdConfigParam2\n"
    ".set reorder\n"
);
