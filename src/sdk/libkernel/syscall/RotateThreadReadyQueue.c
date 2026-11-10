/* EE kernel syscall stub 43 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl RotateThreadReadyQueue\n"
    ".ent RotateThreadReadyQueue\n"
    "RotateThreadReadyQueue:\n"
    "addiu $3, $0, 43\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end RotateThreadReadyQueue\n"
    ".set reorder\n"
);
