/* EE kernel syscall stub 15 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetVInterruptHandler\n"
    ".ent SetVInterruptHandler\n"
    "SetVInterruptHandler:\n"
    "addiu $3, $0, 15\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetVInterruptHandler\n"
    ".set reorder\n"
);
