/* EE kernel syscall stub -72 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iReferSemaStatus\n"
    ".ent iReferSemaStatus\n"
    "iReferSemaStatus:\n"
    "addiu $3, $0, -72\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iReferSemaStatus\n"
    ".set reorder\n"
);
