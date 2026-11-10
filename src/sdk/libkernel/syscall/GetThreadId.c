/* EE kernel syscall stub 47 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GetThreadId\n"
    ".ent GetThreadId\n"
    "GetThreadId:\n"
    "addiu $3, $0, 47\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GetThreadId\n"
    ".set reorder\n"
);
