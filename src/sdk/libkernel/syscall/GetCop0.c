/* EE kernel syscall stub 99 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetCop0\n"
    ".ent GetCop0\n"
    "GetCop0:\n"
    "addiu $3, $0, 99\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetCop0\n"
    ".set reorder\n"
);
