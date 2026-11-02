/* EE kernel syscall stub -106 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl iCpuConfig\n"
    ".ent iCpuConfig\n"
    "iCpuConfig:\n"
    "addiu $3, $0, -106\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end iCpuConfig\n"
    ".set reorder\n"
);
