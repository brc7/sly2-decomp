/* EE kernel syscall stub 68 (hand-written assembly in Sony's libkernel). */
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl s1_WaitSema\n"
    ".ent s1_WaitSema\n"
    "s1_WaitSema:\n"
    "addiu $3, $0, 68\n"
    "syscall\n"
    "jr $31\n"
    "nop\n"
    ".end s1_WaitSema\n"
    ".set reorder\n"
);
