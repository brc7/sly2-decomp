/* EE kernel syscall stub -42 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iChangeThreadPriority\n"
    ".ent iChangeThreadPriority\n"
    "iChangeThreadPriority:\n"
    "addiu $3, $0, -42\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iChangeThreadPriority\n"
    ".set reorder\n"
);
