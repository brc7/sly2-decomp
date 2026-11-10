/* EE kernel syscall stub 119 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl sceSifSetDma\n"
    ".ent sceSifSetDma\n"
    "sceSifSetDma:\n"
    "addiu $3, $0, 119\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end sceSifSetDma\n"
    ".set reorder\n"
);
