/* EE kernel syscall stub 96 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl KSeg0\n"
    ".ent KSeg0\n"
    "KSeg0:\n"
    "addiu $3, $0, 96\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end KSeg0\n"
    ".set reorder\n"
);
