/* EE kernel syscall stub 61 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl SetupHeap\n"
    ".ent SetupHeap\n"
    "SetupHeap:\n"
    "addiu $3, $0, 61\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end SetupHeap\n"
    ".set reorder\n"
);
