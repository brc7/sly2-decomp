/* EE kernel syscall stub 102 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl CpuConfig\n"
    ".ent CpuConfig\n"
    "CpuConfig:\n"
    "addiu $3, $0, 102\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end CpuConfig\n"
    ".set reorder\n"
);
