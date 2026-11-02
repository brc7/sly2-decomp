/* EE kernel syscall stub -255 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl s1_ChangeThreadPriority\n"
    ".ent s1_ChangeThreadPriority\n"
    "s1_ChangeThreadPriority:\n"
    "addiu $3, $0, -255\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end s1_ChangeThreadPriority\n"
    ".set reorder\n"
);
