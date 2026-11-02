/* EE kernel syscall stub -49 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iReferThreadStatus\n"
    ".ent iReferThreadStatus\n"
    "iReferThreadStatus:\n"
    "addiu $3, $0, -49\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iReferThreadStatus\n"
    ".set reorder\n"
);
