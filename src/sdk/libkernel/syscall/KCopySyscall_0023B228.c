/* EE kernel syscall stub 90 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl KCopySyscall_0023B228\n"
    ".ent KCopySyscall_0023B228\n"
    "KCopySyscall_0023B228:\n"
    "addiu $3, $0, 90\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end KCopySyscall_0023B228\n"
    ".set reorder\n"
);
