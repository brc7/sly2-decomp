/* EE kernel syscall stub 109 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetCPUTimer\n"
    ".ent SetCPUTimer\n"
    "SetCPUTimer:\n"
    "addiu $3, $0, 109\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetCPUTimer\n"
    ".set reorder\n"
);
