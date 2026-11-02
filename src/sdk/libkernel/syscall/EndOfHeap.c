/* EE kernel syscall stub 62 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl EndOfHeap\n"
    ".ent EndOfHeap\n"
    "EndOfHeap:\n"
    "addiu $3, $0, 62\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end EndOfHeap\n"
    ".set reorder\n"
);
