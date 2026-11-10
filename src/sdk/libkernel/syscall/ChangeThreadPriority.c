/* EE kernel syscall stub 41 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ChangeThreadPriority\n"
    ".ent ChangeThreadPriority\n"
    "ChangeThreadPriority:\n"
    "addiu $3, $0, 41\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ChangeThreadPriority\n"
    ".set reorder\n"
);
