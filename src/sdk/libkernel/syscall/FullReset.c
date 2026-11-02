/* EE kernel syscall stub 0 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl FullReset\n"
    ".ent FullReset\n"
    "FullReset:\n"
    "addiu $3, $0, 0\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end FullReset\n"
    ".set reorder\n"
);
