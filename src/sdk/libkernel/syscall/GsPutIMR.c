/* EE kernel syscall stub 113 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl GsPutIMR\n"
    ".ent GsPutIMR\n"
    "GsPutIMR:\n"
    "addiu $3, $0, 113\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end GsPutIMR\n"
    ".set reorder\n"
);
