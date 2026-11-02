/* EE kernel syscall stub -103 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iGetCop0\n"
    ".ent iGetCop0\n"
    "iGetCop0:\n"
    "addiu $3, $0, -103\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iGetCop0\n"
    ".set reorder\n"
);
