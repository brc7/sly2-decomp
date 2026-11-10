/* EE kernel syscall stub 65 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl DeleteSema\n"
    ".ent DeleteSema\n"
    "DeleteSema:\n"
    "addiu $3, $0, 65\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end DeleteSema\n"
    ".set reorder\n"
);
