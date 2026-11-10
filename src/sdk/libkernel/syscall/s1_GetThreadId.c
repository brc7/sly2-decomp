/* EE kernel syscall stub 45 (hand-written assembly in Sony's libkernel). */
// Also known as: ReleaseWaitThread (upstream readable / Sly 1 name).
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl s1_GetThreadId\n"
    ".ent s1_GetThreadId\n"
    "s1_GetThreadId:\n"
    "addiu $3, $0, 45\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end s1_GetThreadId\n"
    ".set reorder\n"
);
