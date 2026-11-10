/* EE kernel syscall stub 45 (hand-written assembly in Sony's libkernel). */
// Was s1_GetThreadId: syscall 45 is ReleaseWaitThread (GetThreadId is 47, GetThreadId.c).
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ReleaseWaitThread\n"
    ".ent ReleaseWaitThread\n"
    "ReleaseWaitThread:\n"
    "addiu $3, $0, 45\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ReleaseWaitThread\n"
    ".set reorder\n"
);
