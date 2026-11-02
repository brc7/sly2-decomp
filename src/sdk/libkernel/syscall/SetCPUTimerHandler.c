/* EE kernel syscall stub 108 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetCPUTimerHandler\n"
    ".ent SetCPUTimerHandler\n"
    "SetCPUTimerHandler:\n"
    "addiu $3, $0, 108\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetCPUTimerHandler\n"
    ".set reorder\n"
);
