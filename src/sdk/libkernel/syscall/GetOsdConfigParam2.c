/* EE kernel syscall stub 111 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetOsdConfigParam2\n"
    ".ent GetOsdConfigParam2\n"
    "GetOsdConfigParam2:\n"
    "addiu $3, $0, 111\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetOsdConfigParam2\n"
    ".set reorder\n"
);
