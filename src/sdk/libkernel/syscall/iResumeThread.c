/* EE kernel syscall stub -58 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iResumeThread\n"
    ".ent iResumeThread\n"
    "iResumeThread:\n"
    "addiu $3, $0, -58\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iResumeThread\n"
    ".set reorder\n"
);
