/* EE kernel syscall stub 57 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl ResumeThread\n"
    ".ent ResumeThread\n"
    "ResumeThread:\n"
    "addiu $3, $0, 57\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end ResumeThread\n"
    ".set reorder\n"
);
