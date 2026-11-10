/* EE kernel syscall stub 107 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl sceSifStopDma\n"
    ".ent sceSifStopDma\n"
    "sceSifStopDma:\n"
    "addiu $3, $0, 107\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end sceSifStopDma\n"
    ".set reorder\n"
);
